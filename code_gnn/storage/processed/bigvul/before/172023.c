static char is_inited(void)
{
 char ret;


    pthread_mutex_lock(&state_lock);
    ret = pth != -1;
    pthread_mutex_unlock(&state_lock);

 return ret;
}
