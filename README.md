# data-chunker
Example data transfer

This is an example of packing up a structure of data and transfering it over a TCP connection.

This version needs Visual Studio 2008 and Boost 1.40.0 (installed to `C:\Program Files\boost_1_40_0`, with the libs in the `lib32` subdirectory). To build, just click Visual Studio's build button. 

To run, open two command windows, change to the data-chunker directory and run `Debug\Receiver.exe` then in the other window `Debug\Sender.exe`.
