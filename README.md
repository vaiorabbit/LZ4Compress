# LZ4Compress #

A demonstration program showing how to use LZ4 compression library (https://github.com/Cyan4973/lz4).

*   Created: 2015-12-13 (Sun)
*   Last Modified: 2015-12-13 (Sun)

## Prerequisites ##

Tested on Mac OS X 10.11.2.

    $ clang++ --version                                                                                                [~] 
    Apple LLVM version 7.0.2 (clang-700.1.81)
    Target: x86_64-apple-darwin15.2.0
    Thread model: posix

## Contents ##

*   /external
    *   GetLatestLZ4.sh : Get latest codes.
    *   BuildLibLZ4.sh : Build 'lib/liblz4.a'.

*   /source
    *   Source codes for LZ4Compress command.
    *   Uses /external/lib/liblz4.a
    *   Just hit 'make'. './LZ4Compress -h' shows usage.
    *   Demo:

            $ ./LZ4Compress -in loremipsum.txt
            $ ls loremipsum*
            -rw-r--r--  65536 loremipsum.txt
            -rw-r--r--  26827 loremipsum.txt.lz4.bin


## License ##

LZ4 is available under the terms of 2-clause BSD License.

LZ4Compress is available under the terms of zlib/libpng license.

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.
    
    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:
    
        1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    
        2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    
        3. This notice may not be removed or altered from any source
        distribution.

<!--
Local Variables:
mode: markdown
coding: utf-8
End:
-->
