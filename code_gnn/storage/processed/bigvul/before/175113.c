 virtual int create() {
 int ret;

        ret = mMsg.create(GOOGLE_OUI, WIFI_SUBCMD_SET_COUNTRY_CODE);
 if (ret < 0) {
             ALOGE("Can't create message to send to driver - %d", ret);
 return ret;
 }

        nlattr *data = mMsg.attr_start(NL80211_ATTR_VENDOR_DATA);
        ret = mMsg.put_string(ATTR_COUNTRY_CODE, mCountryCode);
 if (ret < 0) {
 return ret;
 }

        mMsg.attr_end(data);
 return WIFI_SUCCESS;

 }
