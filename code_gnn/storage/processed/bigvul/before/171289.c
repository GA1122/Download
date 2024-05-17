static bool check_mac_perms_from_getcon(pid_t spid, uid_t uid, const char *perm)
{
 if (selinux_enabled <= 0) {
 return true;
 }

 return check_mac_perms(spid, uid, service_manager_context, perm, NULL);
}
