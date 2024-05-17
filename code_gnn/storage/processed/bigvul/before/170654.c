int PreProcessingLib_Create(const effect_uuid_t *uuid,
 int32_t             sessionId,
 int32_t             ioId,
 effect_handle_t *pInterface)
{
    ALOGV("EffectCreate: uuid: %08x session %d IO: %d", uuid->timeLow, sessionId, ioId);

 int status;
 const effect_descriptor_t *desc;
 preproc_session_t *session;
 uint32_t procId;

 if (PreProc_Init() != 0) {
 return sInitStatus;
 }
    desc = PreProc_GetDescriptor(uuid);
 if (desc == NULL) {
        ALOGW("EffectCreate: fx not found uuid: %08x", uuid->timeLow);
 return -EINVAL;
 }
    procId = UuidToProcId(&desc->type);

    session = PreProc_GetSession(procId, sessionId, ioId);
 if (session == NULL) {
        ALOGW("EffectCreate: no more session available");
 return -EINVAL;
 }

    status = Session_CreateEffect(session, procId, pInterface);

 if (status < 0 && session->createdMsk == 0) {
        session->io = 0;
 }
 return status;
}
