#Sniffer

A Sniffer project which based on Qt 4.X and libpcap library

## Some installation information about the software
1. You should install Qt4.x in you system
2. You should install libpcap libpcap and add it to your library path
3. You can also change the Sniffer.pro file to change its config and run qt-make && make to rebuild the project
4. You should run the software in root permission just like sudo ./Sniffer or just change to root(su root)

## The functions of the software
1. List all the availiable net device
2. Select the promiscuous mode to listen or not
3. Select a filter strategy or just edit yourself
4. Just analiysis some protocol such as ethernet,ip, igmp, tcp,udp, http, ssdp, the other protocol may add
in the future.
5. Haven't added the file saving and loading functions because of the time, so the file saving and loading 
functions in the toolbar or the menu is no use.
6. Search the packet by adding the search string in the search line text, just support simple Regex, 
For example: you can add protocol = tcp to select the tcp packets
7. Zoom the viewport
8. Highlight the byte view or char view, by trigger the prototree node
9. Select hex view or bit view in the contextmenu of the raw view widget in the bottom
