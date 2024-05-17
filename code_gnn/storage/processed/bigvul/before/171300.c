int svcmgr_handler(struct binder_state *bs,
 struct binder_transaction_data *txn,
 struct binder_io *msg,
 struct binder_io *reply)
{
 struct svcinfo *si;
 uint16_t *s;
 size_t len;
 uint32_t handle;
 uint32_t strict_policy;
 int allow_isolated;


 if (txn->target.ptr != BINDER_SERVICE_MANAGER)
 return -1;

 if (txn->code == PING_TRANSACTION)
 return 0;

    strict_policy = bio_get_uint32(msg);
    s = bio_get_string16(msg, &len);
 if (s == NULL) {
 return -1;
 }

 if ((len != (sizeof(svcmgr_id) / 2)) ||
        memcmp(svcmgr_id, s, sizeof(svcmgr_id))) {
        fprintf(stderr,"invalid id %s\n", str8(s, len));
 return -1;
 }

 if (sehandle && selinux_status_updated() > 0) {
 struct selabel_handle *tmp_sehandle = selinux_android_service_context_handle();
 if (tmp_sehandle) {
            selabel_close(sehandle);
            sehandle = tmp_sehandle;
 }
 }

 switch(txn->code) {
 case SVC_MGR_GET_SERVICE:
 case SVC_MGR_CHECK_SERVICE:
        s = bio_get_string16(msg, &len);
 if (s == NULL) {
 return -1;
 }
        handle = do_find_service(s, len, txn->sender_euid, txn->sender_pid);
 if (!handle)
 break;
        bio_put_ref(reply, handle);
 return 0;

 case SVC_MGR_ADD_SERVICE:
        s = bio_get_string16(msg, &len);
 if (s == NULL) {
 return -1;
 }
        handle = bio_get_ref(msg);
        allow_isolated = bio_get_uint32(msg) ? 1 : 0;
 if (do_add_service(bs, s, len, handle, txn->sender_euid,
            allow_isolated, txn->sender_pid))
 return -1;
 break;

 case SVC_MGR_LIST_SERVICES: {
 uint32_t n = bio_get_uint32(msg);

 if (!svc_can_list(txn->sender_pid, txn->sender_euid)) {
            ALOGE("list_service() uid=%d - PERMISSION DENIED\n",
                    txn->sender_euid);
 return -1;
 }
        si = svclist;
 while ((n-- > 0) && si)
            si = si->next;
 if (si) {
            bio_put_string16(reply, si->name);
 return 0;
 }
 return -1;
 }
 default:
        ALOGE("unknown code %d\n", txn->code);
 return -1;
 }

    bio_put_uint32(reply, 0);
 return 0;
}
