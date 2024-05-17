void* message_thread(void *input)
{
    omx_vdec* omx = reinterpret_cast<omx_vdec*>(input);
 unsigned char id;
 int n;

    DEBUG_PRINT_HIGH("omx_vdec: message thread start");
    prctl(PR_SET_NAME, (unsigned long)"VideoDecMsgThread", 0, 0, 0);
 while (1) {

        n = read(omx->m_pipe_in, &id, 1);

 if (0 == n) {
 break;
 }

 if (1 == n) {
            omx->process_event_cb(omx, id);
 }
 if ((n < 0) && (errno != EINTR)) {
            DEBUG_PRINT_LOW("ERROR: read from pipe failed, ret %d errno %d", n, errno);
 break;
 }
 }
    DEBUG_PRINT_HIGH("omx_vdec: message thread stop");
 return 0;
}
