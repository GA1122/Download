uint32_t do_find_service(const uint16_t *s, size_t len, uid_t uid, pid_t spid)
{
 struct svcinfo *si = find_svc(s, len);

 if (!si || !si->handle) {
 return 0;
 }

 if (!si->allow_isolated) {
 uid_t appid = uid % AID_USER;
 if (appid >= AID_ISOLATED_START && appid <= AID_ISOLATED_END) {
 return 0;
 }
 }

 if (!svc_can_find(s, len, spid, uid)) {
 return 0;
 }

 return si->handle;
}
