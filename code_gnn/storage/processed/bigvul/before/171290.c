static bool check_mac_perms_from_lookup(pid_t spid, uid_t uid, const char *perm, const char *name)
{
 bool allowed;
 char *tctx = NULL;

 if (selinux_enabled <= 0) {
 return true;
 }

 if (!sehandle) {
        ALOGE("SELinux: Failed to find sehandle. Aborting service_manager.\n");
        abort();
 }

 if (selabel_lookup(sehandle, &tctx, name, 0) != 0) {
        ALOGE("SELinux: No match for %s in service_contexts.\n", name);
 return false;
 }

    allowed = check_mac_perms(spid, uid, tctx, perm, name);
    freecon(tctx);
 return allowed;
}
