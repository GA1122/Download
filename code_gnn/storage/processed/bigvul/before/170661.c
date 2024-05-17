int Session_Init(preproc_session_t *session)
{
 size_t i;
 int status = 0;

    session->state = PREPROC_SESSION_STATE_INIT;
    session->id = 0;
    session->io = 0;
    session->createdMsk = 0;
    session->apm = NULL;
 for (i = 0; i < PREPROC_NUM_EFFECTS && status == 0; i++) {
        status = Effect_Init(&session->effects[i], i);
 }
 return status;
}
