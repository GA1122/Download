 virtual int create() {
 int ret;

        ret = mMsg.create(GOOGLE_OUI, WIFI_SUBCMD_NODFS_SET);
 if (ret < 0) {
            ALOGE("Can't create message to send to driver - %d", ret);
 return ret;
 }

        nlattr *data = mMsg.attr_start(NL80211_ATTR_VENDOR_DATA);
        ret = mMsg.put_u32(ATTR_NODFS_VALUE, mNoDfs);
 if (ret < 0) {
 return ret;
 }

        mMsg.attr_end(data);
 return WIFI_SUCCESS;
 }
