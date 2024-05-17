int do_add_service(struct binder_state *bs,
 const uint16_t *s, size_t len,
 uint32_t handle, uid_t uid, int allow_isolated,
 pid_t spid)
{
 struct svcinfo *si;


 if (!handle || (len == 0) || (len > 127))
 return -1;

 if (!svc_can_register(s, len, spid, uid)) {
        ALOGE("add_service('%s',%x) uid=%d - PERMISSION DENIED\n",
             str8(s, len), handle, uid);
 return -1;
 }

    si = find_svc(s, len);
 if (si) {
 if (si->handle) {
            ALOGE("add_service('%s',%x) uid=%d - ALREADY REGISTERED, OVERRIDE\n",
                 str8(s, len), handle, uid);
            svcinfo_death(bs, si);
 }
        si->handle = handle;
 } else {
        si = malloc(sizeof(*si) + (len + 1) * sizeof(uint16_t));
 if (!si) {
            ALOGE("add_service('%s',%x) uid=%d - OUT OF MEMORY\n",
                 str8(s, len), handle, uid);
 return -1;
 }
        si->handle = handle;
        si->len = len;
        memcpy(si->name, s, (len + 1) * sizeof(uint16_t));
        si->name[len] = '\0';
        si->death.func = (void*) svcinfo_death;
        si->death.ptr = si;
        si->allow_isolated = allow_isolated;
        si->next = svclist;
        svclist = si;
 }

    binder_acquire(bs, handle);
    binder_link_to_death(bs, handle, &si->death);
 return 0;
}
