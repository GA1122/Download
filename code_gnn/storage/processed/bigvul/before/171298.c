static int svc_can_list(pid_t spid, uid_t uid)
{
 const char *perm = "list";
 return check_mac_perms_from_getcon(spid, uid, perm) ? 1 : 0;
}
