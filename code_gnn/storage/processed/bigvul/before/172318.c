jboolean setSSIDField(JNIHelper helper, jobject scanResult, const char *rawSsid) {

 int len = strlen(rawSsid);

 if (len > 0) {
 JNIObject<jbyteArray> ssidBytes = helper.newByteArray(len);
        helper.setByteArrayRegion(ssidBytes, 0, len, (jbyte *) rawSsid);
        jboolean ret = helper.callStaticMethod(mCls,
 "setSsid", "([BLandroid/net/wifi/ScanResult;)Z", ssidBytes.get(), scanResult);
 return ret;
 } else {
 return true;
 }
}
