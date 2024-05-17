static void a2dp_stream_common_init(struct a2dp_stream_common *common)
{
 pthread_mutexattr_t lock_attr;

    FNLOG();

    pthread_mutexattr_init(&lock_attr);
    pthread_mutexattr_settype(&lock_attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&common->lock, &lock_attr);

    common->ctrl_fd = AUDIO_SKT_DISCONNECTED;
    common->audio_fd = AUDIO_SKT_DISCONNECTED;
    common->state = AUDIO_A2DP_STATE_STOPPED;

  
    common->buffer_sz = AUDIO_STREAM_OUTPUT_BUFFER_SZ;
}
