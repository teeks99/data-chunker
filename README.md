# data-chunker
Example data transfer

This is an example of packing up a structure of data and transfering it over a TCP connection.

This version needs Visual Studio 2015 and [boost 1.62.0 (with appropriate libs)](https://sourceforge.net/projects/boost/files/boost-binaries/1.62.0/) (Installed in `C:\local\boost_1_62_0` with libs in `lib32-msvc-14.0`). To build just click visual studio's build button.

To run, open two command windows, change to the data-chunker directory and run `Debug\Receiver.exe` then in the other window `Debug\Sender.exe`.
