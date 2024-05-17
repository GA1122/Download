 virtual void ErrorMessage(const String16 message) {
 String8 m8(message);
    std::string mstd(m8.string());

    ALOGD("PAC-error: %s\n", mstd.c_str());  
    errors.push_back(mstd);
 }
