static int uipc_main_init(void)
{
 int i;
 pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&uipc_main.mutex, &attr);

    BTIF_TRACE_EVENT("### uipc_main_init ###");

  
 if (socketpair(AF_UNIX, SOCK_STREAM, 0, uipc_main.signal_fds) < 0)
 {
 return -1;
 }

    FD_SET(uipc_main.signal_fds[0], &uipc_main.active_set);
    uipc_main.max_fd = MAX(uipc_main.max_fd, uipc_main.signal_fds[0]);

 for (i=0; i< UIPC_CH_NUM; i++)
 {
        tUIPC_CHAN *p = &uipc_main.ch[i];
        p->srvfd = UIPC_DISCONNECTED;
        p->fd = UIPC_DISCONNECTED;
        p->task_evt_flags = 0;
        pthread_cond_init(&p->cond, NULL);
        pthread_mutex_init(&p->cond_mutex, NULL);
        p->cback = NULL;
 }

 return 0;
}
