static bool parseMacAddress(JNIEnv *env, jobject obj, mac_addr addr) {
 JNIHelper helper(env);
 JNIObject<jstring> macAddrString = helper.getStringField(obj, "bssid");
 if (macAddrString == NULL) {
        ALOGE("Error getting bssid field");
 return false;
 }

 ScopedUtfChars chars(env, macAddrString);
 const char *bssid = chars.c_str();
 if (bssid == NULL) {
        ALOGE("Error getting bssid");
 return false;
 }

    parseMacAddress(bssid, addr);
 return true;
}
