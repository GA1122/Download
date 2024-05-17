static bool drop_privileges() {
 gid_t groups[] = { AID_DEBUGGERD, AID_LOG, AID_READPROC };
 if (setgroups(sizeof(groups)/sizeof(groups[0]), groups) != 0) {
    ALOGE("debuggerd: failed to setgroups: %s", strerror(errno));
 return false;
 }

 if (setresgid(AID_DEBUGGERD, AID_DEBUGGERD, AID_DEBUGGERD) != 0) {
    ALOGE("debuggerd: failed to setresgid: %s", strerror(errno));
 return false;
 }

 if (setresuid(AID_DEBUGGERD, AID_DEBUGGERD, AID_DEBUGGERD) != 0) {
    ALOGE("debuggerd: failed to setresuid: %s", strerror(errno));
 return false;
 }

 return true;
}
