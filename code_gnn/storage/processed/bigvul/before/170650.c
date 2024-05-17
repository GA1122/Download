preproc_session_t *PreProc_GetSession(int32_t procId, int32_t  sessionId, int32_t  ioId)
{
 size_t i;
 int free = -1;
 for (i = 0; i < PREPROC_NUM_SESSIONS; i++) {
 if (sSessions[i].io == ioId) {
 if (sSessions[i].createdMsk & (1 << procId)) {
 return NULL;
 }
 return &sSessions[i];
 }
 }
 for (i = 0; i < PREPROC_NUM_SESSIONS; i++) {
 if (sSessions[i].io == 0) {
            sSessions[i].id = sessionId;
            sSessions[i].io = ioId;
 return &sSessions[i];
 }
 }
 return NULL;
}
