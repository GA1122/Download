eventLoop(void *param) {
 int ret;
 int filedes[2];

    ril_event_init();

    pthread_mutex_lock(&s_startupMutex);

    s_started = 1;
    pthread_cond_broadcast(&s_startupCond);

    pthread_mutex_unlock(&s_startupMutex);

    ret = pipe(filedes);

 if (ret < 0) {
        RLOGE("Error in pipe() errno:%d", errno);
 return NULL;
 }

    s_fdWakeupRead = filedes[0];
    s_fdWakeupWrite = filedes[1];

    fcntl(s_fdWakeupRead, F_SETFL, O_NONBLOCK);

    ril_event_set (&s_wakeupfd_event, s_fdWakeupRead, true,
                processWakeupCallback, NULL);

    rilEventAddWakeup (&s_wakeupfd_event);

    ril_event_loop();
    RLOGE ("error in event_loop_base errno:%d", errno);
    kill(0, SIGKILL);

 return NULL;
}
