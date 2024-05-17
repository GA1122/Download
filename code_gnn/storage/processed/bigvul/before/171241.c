status_t SoundTriggerHwService::dump(int fd, const Vector<String16>& args __unused) {
 String8 result;
 if (checkCallingPermission(String16("android.permission.DUMP")) == false) {
        result.appendFormat("Permission Denial: can't dump SoundTriggerHwService");
        write(fd, result.string(), result.size());
 } else {
 bool locked = tryLock(mServiceLock);
 if (!locked) {
            result.append("SoundTriggerHwService may be deadlocked\n");
            write(fd, result.string(), result.size());
 }

 if (locked) mServiceLock.unlock();
 }
 return NO_ERROR;
}
