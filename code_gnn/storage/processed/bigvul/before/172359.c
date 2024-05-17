void* message_thread(void *input)
{
    omx_video* omx = reinterpret_cast<omx_video*>(input);
 unsigned char id;
 int n;

    DEBUG_PRINT_LOW("omx_venc: message thread start");
    prctl(PR_SET_NAME, (unsigned long)"VideoEncMsgThread", 0, 0, 0);
 while (1) {
        n = read(omx->m_pipe_in, &id, 1);
 if (0 == n) {
 break;
 }

 if (1 == n) {
            omx->process_event_cb(omx, id);
 }
#ifdef QLE_BUILD
 if (n < 0) break;
#else
 if ((n < 0) && (errno != EINTR)) break;
#endif
 }
    DEBUG_PRINT_LOW("omx_venc: message thread stop");
 return 0;
}
