 SetPnoMacAddrOuiCommand(wifi_interface_handle handle, oui scan_oui)
 : WifiCommand("SetPnoMacAddrOuiCommand", handle, 0)
 {
        mOui = scan_oui;
 }
