static jstring android_net_wifi_waitForEvent(JNIEnv* env, jobject)
{
 char buf[EVENT_BUF_SIZE];
 int nread = ::wifi_wait_for_event(buf, sizeof buf);
 if (nread > 0) {
 return env->NewStringUTF(buf);
 } else {
 return NULL;
 }
}
