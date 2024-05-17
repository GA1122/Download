static int lib_create(const effect_uuid_t *uuid,
 int32_t             sessionId,
 int32_t             ioId,
 effect_handle_t *pInterface)
{
    ALOGV("lib_create: uuid: %08x session %d IO: %d", uuid->timeLow, sessionId, ioId);

 int status;
 const effect_descriptor_t *desc;
 struct session_s *session;
 uint32_t id;

 if (init() != 0)
 return init_status;

    desc =  get_descriptor(uuid);

 if (desc == NULL) {
        ALOGW("lib_create: fx not found uuid: %08x", uuid->timeLow);
 return -EINVAL;
 }
    id = uuid_to_id(&desc->type);

    session = get_session(id, sessionId, ioId);

 if (session == NULL) {
        ALOGW("lib_create: no more session available");
 return -EINVAL;
 }

    status = session_create_effect(session, id, pInterface);

 if (status < 0 && session->created_msk == 0) {
        list_remove(&session->node);
        free(session);
 }
 return status;
}
