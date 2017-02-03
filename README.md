# sniffer
To build program run make all - you will receive 2 files snifferd and snifferctl. To install them into system and load sniffer at boot 
type sudo make install.
After that you can controll sniffer using sudo snifferctl [commands](see sudo snifferctl --help). It is not recomended to start sniffer 
using sudo service snifferd start and stop it using snifferctl. In this case you should run sudo service snifferd stop first.
