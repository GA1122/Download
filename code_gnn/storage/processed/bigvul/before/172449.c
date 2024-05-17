unsigned venc_dev::venc_resume(void)
{
    pthread_mutex_lock(&pause_resume_mlock);
    paused = false;
    pthread_mutex_unlock(&pause_resume_mlock);

 return pthread_cond_signal(&pause_resume_cond);
}
