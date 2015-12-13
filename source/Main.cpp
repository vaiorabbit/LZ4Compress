#include <memory>
#include <fstream>
#include <iostream>
#include <string>

#include "lz4.h"

#include "Types.h"
#include "Utility.h"
#include "Format.h"
#include "Option.h"
#include "Convert.h"

namespace command
{
    struct Entry
    {
        const char* name;
        const char* argdesc;
        const char* usage;
    };

    typedef enum {
        eID_Help,
        eID_InputFilename,
        eID_OutputFilename,
        eID_DecompressMode,
        eID_Endian,
        eID_BlockSize,

        eID_Max,
        eID_Unknown
    } CommandID;

    static const Entry opts[eID_Max] = {
        {"-h", "", "show this usage."},
        {"-in", "[filename]", "input filename."},
        {"-out", "[filename]", "output filename."},
        {"-d", "", "decompress mode"},
        {"-endian", "", "swap endian"},
        {"-block", "[8192|16384|...]", "Do per-block compression. default=16384 (16 * 1024)"},
    };
} // namespace command

namespace app
{
    static std::string strInFilename;
    static std::string strOutFilename;

    static std::ifstream fIn;
    static std::ofstream fOut;

    static bool bModeDecompress = false;
    static bool bModeEndianSwap = false;

    static uint32 unBlockSize = 16 * 1024;

    static int ParseOptions( int argc, char* argv[] )
    {
        // Show help
        if ( argc == 1 || OptionExists( argc, argv, command::opts[command::eID_Help].name ) )
        {
            std::cout << "LZ4Compress Ver. " << __DATE__ << std::endl;
            for ( int i = 0; i < command::eID_Max; ++i )
            {
                std::cout << "\t" << command::opts[i].name << ' ' << command::opts[i].argdesc << "\t:" << command::opts[i].usage << std::endl;
            }

            return 1;
        }

        // Run as decompress mode
        if ( OptionExists( argc, argv, command::opts[command::eID_DecompressMode].name ) )
        {
            app::bModeDecompress = true;
        }

        if ( OptionExists( argc, argv, command::opts[command::eID_InputFilename].name ) )
        {
            app::strInFilename = OptionValue( argc, argv, command::opts[command::eID_InputFilename].name );
        }
        else
        {
            std::cout << "[ERROR] LZ4Compress : strInFilename = NULL" << std::endl;
            return -1;
        }

        // Build output filename
        if ( OptionExists( argc, argv, command::opts[command::eID_OutputFilename].name ) )
        {
            app::strOutFilename = OptionValue( argc, argv, command::opts[command::eID_OutputFilename].name );
        }
        else
        {
            if ( app::bModeDecompress )
            {
                std::cout << "[ERROR] LZ4Compress : strOutFilename = NULL" << std::endl;
                return -1;
            }
            else
            {
                // LZ4 compress -> append ".lz4.bin" suffix.
                app::strOutFilename = app::strInFilename + ".lz4.bin";
            }
        }

        // Check endian swap mode
        if ( OptionExists( argc, argv, command::opts[command::eID_Endian].name ) )
        {
            app::bModeEndianSwap = true;
        }

        // Get block size for streaming decompression
        if ( OptionExists( argc, argv, command::opts[command::eID_BlockSize].name ) )
        {
            char* pstrBlockSize = OptionValue( argc, argv, command::opts[command::eID_BlockSize].name );
            if ( pstrBlockSize != NULL && Convert::IsDecimal(pstrBlockSize) )
                app::unBlockSize = Convert::ToDecimal(pstrBlockSize);
            else
                app::unBlockSize = 16 * 1024;
        }

        return 0;
    }

    static int Decompress()
    {
        // Open input
        try
        {
            app::fIn.open( app::strInFilename.c_str(), std::ios_base::in | std::ios_base::binary );
        }
        catch ( std::exception& e )
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }

        // Open output
        try
        {
            app::fOut.open( app::strOutFilename.c_str(), std::ios_base::out | std::ios_base::binary );
        }
        catch ( std::exception& e )
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }

        bool bError = false;

        BinaryHeader header;
        app::fIn.read( (char*)&header, sizeof(BinaryHeader) );

        app::unBlockSize = header.BlockSize;
        uint32 unOutFileSize = header.FileSize;
        uint32 unBlockCount = header.BlockCount;

        // Write decompressed content here
        std::unique_ptr<char[]> pOutFileContent( new char[unOutFileSize] );

        // Read compressed segment here
        std::unique_ptr<char[]> pSegmentContent( new char[app::unBlockSize] );

        LZ4_streamDecode_t cLZ4StreamDecode;
        LZ4_setStreamDecode(&cLZ4StreamDecode, NULL, 0);
        for ( uint32 i = 0; i < unBlockCount; ++i )
        {
            uint32 unSegmentSize;
            app::fIn.read( (char*)&unSegmentSize, sizeof(uint32) );
            app::fIn.read( (char*)pSegmentContent.get(), unSegmentSize );

            int nDecompressedSize = LZ4_decompress_safe_continue(
                &cLZ4StreamDecode,
                pSegmentContent.get(),
                pOutFileContent.get()+app::unBlockSize * i,
                unSegmentSize,
                app::unBlockSize);
            if ( nDecompressedSize <= 0 )
            {
                bError = true;
                break;
            }
        }

        if ( !bError )
        {
            app::fOut.write( (char*)pOutFileContent.get(), unOutFileSize );
        }

        return bError ? -1 : 0;
    }

    static int Compress()
    {
        // Open input
        try
        {
            app::fIn.open( app::strInFilename.c_str(), std::ios_base::in | std::ios_base::binary );
        }
        catch ( std::exception& e )
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }

        // Open output
        try
        {
            app::fOut.open( app::strOutFilename.c_str(), std::ios_base::out | std::ios_base::binary );
        }
        catch ( std::exception& e )
        {
            std::cerr << e.what() << std::endl;
            return -1;
        }

        bool bError = false;

        // Calculate file size
        std::streampos nInFileSize = app::fIn.tellg();
        app::fIn.seekg( 0, std::ios_base::end );
        nInFileSize = app::fIn.tellg() - nInFileSize;
        app::fIn.seekg( 0, std::ios_base::beg ); // rewind

        // Read whole input
        std::unique_ptr<char[]> pInFileContent( new char[nInFileSize] );
        app::fIn.read( (char*)pInFileContent.get(), nInFileSize );

        // Do compress
        {
            uint32 unBlockCount = nInFileSize / app::unBlockSize;
            if ( nInFileSize % app::unBlockSize != 0 )
            {
                ++unBlockSize;
            }
            BinaryHeader header;
            WriteFourCC(   &header.FourCC, 'L','Z','4','A' );
            WriteValueU32( &header.FileSize, nInFileSize, app::bModeEndianSwap );
            WriteValueU32( &header.BlockSize, app::unBlockSize, app::bModeEndianSwap );
            WriteValueU32( &header.BlockCount, unBlockCount, app::bModeEndianSwap );
            app::fOut.write( (char*)&header, sizeof(BinaryHeader) );

            LZ4_stream_t cLZ4Stream;
            LZ4_resetStream(&cLZ4Stream);
            size_t nCurrentInOffset = 0;
            size_t nAvailableFileSize = nInFileSize;
            while ( nAvailableFileSize > 0 )
            {
                int nOutFileContentSize = LZ4_COMPRESSBOUND(app::unBlockSize);
                std::unique_ptr<char[]> pOutFileContent( new char[nOutFileContentSize] );
                int nOutFileSize = LZ4_compress_fast_continue(
                    &cLZ4Stream,
                    pInFileContent.get()+nCurrentInOffset,
                    pOutFileContent.get(),
                    nAvailableFileSize >= app::unBlockSize ? app::unBlockSize : nAvailableFileSize,
                    nOutFileContentSize,
                    1);
                if ( nOutFileSize <= 0 )
                {
                    bError = true;
                    break;
                }
                uint32 unOutSize;
                WriteValueU32( &unOutSize, nOutFileSize, app::bModeEndianSwap );
                app::fOut.write( (char*)&unOutSize, sizeof(uint32) );
                app::fOut.write( (char*)pOutFileContent.get(), nOutFileSize );

                if ( nAvailableFileSize >= app::unBlockSize )
                {
                    nCurrentInOffset += app::unBlockSize;
                    nAvailableFileSize -= app::unBlockSize;
                }
                else
                {
                    nCurrentInOffset += (app::unBlockSize - nAvailableFileSize);
                    nAvailableFileSize = 0;
                }
            }
        }
        app::fIn.close();
        app::fOut.close();

        return bError ? -1 : 0;
    }

} // namespace app



int main( int argc, char* argv[] )
{
    int nParseResult = app::ParseOptions( argc, argv );
    if ( nParseResult != 0 )
        return nParseResult;

    int nOperationResult = app::bModeDecompress ? app::Decompress() : app::Compress();
    return nOperationResult;
}
