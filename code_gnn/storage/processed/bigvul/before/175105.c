 GetMulticastIdCommand(wifi_handle handle, const char *name, const char *group)
 : WifiCommand("GetMulticastIdCommand", handle, 0)
 {
        mName = name;
        mGroup = group;
        mId = -1;
 }
