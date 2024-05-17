 SetNodfsCommand(wifi_interface_handle handle, u32 nodfs)
 : WifiCommand("SetNodfsCommand", handle, 0) {
        mNoDfs = nodfs;
 }
