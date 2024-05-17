 virtual void AlertMessage(String16 message) {
 String8 m8(message);
    std::string mstd(m8.string());

    ALOGD("PAC-alert: %s\n", mstd.c_str());  
    alerts.push_back(mstd);
 }
