 int createRequest(WifiRequest& request, int subcmd, byte *scan_oui) {
 int result = request.create(GOOGLE_OUI, subcmd);
 if (result < 0) {
 return result;
 }

        nlattr *data = request.attr_start(NL80211_ATTR_VENDOR_DATA);
        result = request.put(ANDR_WIFI_ATTRIBUTE_PNO_RANDOM_MAC_OUI, scan_oui, DOT11_OUI_LEN);
 if (result < 0) {
 return result;
 }

        request.attr_end(data);
 return WIFI_SUCCESS;

 }
