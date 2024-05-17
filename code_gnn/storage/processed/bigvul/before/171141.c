static bool checkPermission(const char* permissionString) {
 if (getpid() == IPCThreadState::self()->getCallingPid()) return true;
 bool ok = checkCallingPermission(String16(permissionString));
 if (!ok) ALOGE("Request requires %s", permissionString);
 return ok;
}
