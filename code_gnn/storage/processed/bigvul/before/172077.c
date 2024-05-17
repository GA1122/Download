static inline int create_thread(void *(*start_routine)(void *), void * arg,
 pthread_t * thread_id)
{
 pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
 int policy;
 int min_pri=0;
 int ret = -1;
 struct sched_param param;

 if ((ret = pthread_create(thread_id, &thread_attr, start_routine, arg))!=0 )
 {
        APPL_TRACE_ERROR("pthread_create : %s", strerror(errno));
 return ret;
 }
  
    pthread_getschedparam(*thread_id, &policy, &param);
    min_pri = sched_get_priority_min(policy);
 if (param.sched_priority > min_pri) {
        param.sched_priority -= 1;
 }
    pthread_setschedparam(*thread_id, policy, &param);
 return ret;
}
