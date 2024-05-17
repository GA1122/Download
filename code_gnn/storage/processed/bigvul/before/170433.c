bool Parcel::enforceInterface(const String16& interface,
 IPCThreadState* threadState) const
{
 int32_t strictPolicy = readInt32();
 if (threadState == NULL) {
        threadState = IPCThreadState::self();
 }
 if ((threadState->getLastTransactionBinderFlags() &
 IBinder::FLAG_ONEWAY) != 0) {
      threadState->setStrictModePolicy(0);
 } else {
      threadState->setStrictModePolicy(strictPolicy);
 }
 const String16 str(readString16());
 if (str == interface) {
 return true;
 } else {
        ALOGW("**** enforceInterface() expected '%s' but read '%s'",
 String8(interface).string(), String8(str).string());
 return false;
 }
}
