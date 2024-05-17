static enum test_return test_binary_pipeline_hickup(void)
{
    size_t buffersize = 65 * 1024;
    void *buffer = malloc(buffersize);
    int ii;

    pthread_t tid;
    int ret;
    allow_closed_read = true;
    hickup_thread_running = true;
    if ((ret = pthread_create(&tid, NULL,
                              binary_hickup_recv_verification_thread, NULL)) != 0) {
        fprintf(stderr, "Can't create thread: %s\n", strerror(ret));
        return TEST_FAIL;
    }

     
    usleep(250);

    srand((int)time(NULL));
    for (ii = 0; ii < 2; ++ii) {
        test_binary_pipeline_hickup_chunk(buffer, buffersize);
    }

     
    size_t len = raw_command(buffer, buffersize, PROTOCOL_BINARY_CMD_QUITQ,
                             NULL, 0, NULL, 0);
    safe_send(buffer, len, false);

    pthread_join(tid, NULL);
    free(buffer);
    return TEST_PASS;
}