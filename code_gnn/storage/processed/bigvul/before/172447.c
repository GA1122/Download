unsigned venc_dev::venc_pause(void)
{
    pthread_mutex_lock(&pause_resume_mlock);
    paused = true;
    pthread_mutex_unlock(&pause_resume_mlock);
 return 0;
}
