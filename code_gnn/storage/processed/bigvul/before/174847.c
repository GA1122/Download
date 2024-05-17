static int getCallingUid() {
 return IPCThreadState::self()->getCallingUid();
}
