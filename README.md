## This programs are made for text file compression and decompression.
 The compression are made by using Huffman's algorithm, and then its generate folder with compressed bits from file and with dictionary to make it decompress later.
 The decompress program is reading dictionary and compressed bits from folder and then its using Trie data structure to revers compression data to normal data.
#### To use this programs u have to first compile them by using `./compile.sh script` 
#### If you have permission denied error type to console `chmod 777 compile.sh`
## To `compress` 
  use `./compress <fileToCompress>` for example `./compress test.txt`
## To`decompress` 
  use `./decompress <folderWithCompressedData>` for example `./decompress text`
