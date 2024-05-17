 SetCountryCodeCommand(wifi_interface_handle handle, const char *country_code)
 : WifiCommand("SetCountryCodeCommand", handle, 0) {
        mCountryCode = country_code;
 }
