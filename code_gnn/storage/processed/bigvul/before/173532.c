int lib_init()
{
    pthread_once(&once, init_once);
 return init_status;
}
