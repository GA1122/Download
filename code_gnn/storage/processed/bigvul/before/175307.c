static void processCommandsCallback(int fd, short flags, void *param) {
 RecordStream *p_rs;
 void *p_record;
 size_t recordlen;
 int ret;
 SocketListenParam *p_info = (SocketListenParam *)param;

    assert(fd == p_info->fdCommand);

    p_rs = p_info->p_rs;

 for (;;) {
  
        ret = record_stream_get_next(p_rs, &p_record, &recordlen);

 if (ret == 0 && p_record == NULL) {
  
 break;
 } else if (ret < 0) {
 break;
 } else if (ret == 0) {  
            processCommandBuffer(p_record, recordlen, p_info->socket_id);
 }
 }

 if (ret == 0 || !(errno == EAGAIN || errno == EINTR)) {
  
 if (ret != 0) {
            RLOGE("error on reading command socket errno:%d\n", errno);
 } else {
            RLOGW("EOS.  Closing command socket.");
 }

        close(fd);
        p_info->fdCommand = -1;

        ril_event_del(p_info->commands_event);

        record_stream_free(p_rs);

  
        rilEventAddWakeup(&s_listen_event);

        onCommandsSocketClosed(p_info->socket_id);
 }
}
