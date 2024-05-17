static void listenCallback (int fd, short flags, void *param) {
 int ret;
 int err;
 int is_phone_socket;
 int fdCommand = -1;
 const char* processName;
 RecordStream *p_rs;
 MySocketListenParam* listenParam;
 RilSocket *sapSocket = NULL;
    socketClient *sClient = NULL;

 SocketListenParam *p_info = (SocketListenParam *)param;

 if(RIL_SAP_SOCKET == p_info->type) {
        listenParam = (MySocketListenParam *)param;
        sapSocket = listenParam->socket;
 }

 struct sockaddr_un peeraddr;
 socklen_t socklen = sizeof (peeraddr);

 struct ucred creds;
 socklen_t szCreds = sizeof(creds);

 struct passwd *pwd = NULL;

 if(NULL == sapSocket) {
        assert (*p_info->fdCommand < 0);
        assert (fd == *p_info->fdListen);
        processName = PHONE_PROCESS;
 } else {
        assert (sapSocket->commandFd < 0);
        assert (fd == sapSocket->listenFd);
        processName = BLUETOOTH_PROCESS;
 }


    fdCommand = accept(fd, (sockaddr *) &peeraddr, &socklen);

 if (fdCommand < 0 ) {
        RLOGE("Error on accept() errno:%d", errno);
  
 if(NULL == sapSocket) {
            rilEventAddWakeup(p_info->listen_event);
 } else {
            rilEventAddWakeup(sapSocket->getListenEvent());
 }
 return;
 }

  
    errno = 0;
    is_phone_socket = 0;

    err = getsockopt(fdCommand, SOL_SOCKET, SO_PEERCRED, &creds, &szCreds);

 if (err == 0 && szCreds > 0) {
        errno = 0;
        pwd = getpwuid(creds.uid);
 if (pwd != NULL) {
 if (strcmp(pwd->pw_name, processName) == 0) {
                is_phone_socket = 1;
 } else {
                RLOGE("RILD can't accept socket from process %s", pwd->pw_name);
 }
 } else {
            RLOGE("Error on getpwuid() errno: %d", errno);
 }
 } else {
        RLOGD("Error on getsockopt() errno: %d", errno);
 }

 if (!is_phone_socket) {
        RLOGE("RILD must accept socket from %s", processName);

        close(fdCommand);
        fdCommand = -1;

 if(NULL == sapSocket) {
            onCommandsSocketClosed(p_info->socket_id);

  
            rilEventAddWakeup(p_info->listen_event);
 } else {
            sapSocket->onCommandsSocketClosed();

  
            rilEventAddWakeup(sapSocket->getListenEvent());
 }

 return;
 }

    ret = fcntl(fdCommand, F_SETFL, O_NONBLOCK);

 if (ret < 0) {
        RLOGE ("Error setting O_NONBLOCK errno:%d", errno);
 }

 if(NULL == sapSocket) {
        RLOGI("libril: new connection to %s", rilSocketIdToString(p_info->socket_id));

        p_info->fdCommand = fdCommand;
        p_rs = record_stream_new(p_info->fdCommand, MAX_COMMAND_BYTES);
        p_info->p_rs = p_rs;

        ril_event_set (p_info->commands_event, p_info->fdCommand, 1,
        p_info->processCommandsCallback, p_info);
        rilEventAddWakeup (p_info->commands_event);

        onNewCommandConnect(p_info->socket_id);
 } else {
        RLOGI("libril: new connection");

        sapSocket->setCommandFd(fdCommand);
        p_rs = record_stream_new(sapSocket->getCommandFd(), MAX_COMMAND_BYTES);
        sClient = new socketClient(sapSocket,p_rs);
        ril_event_set (sapSocket->getCallbackEvent(), sapSocket->getCommandFd(), 1,
        sapSocket->getCommandCb(), sClient);

        rilEventAddWakeup(sapSocket->getCallbackEvent());
        sapSocket->onNewCommandConnect();
 }
}
