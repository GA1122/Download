bool ProCamera2Client::hasExclusiveLock() {
 Mutex::Autolock icl(mBinderSerializationLock);
 return mExclusiveLock;
}
