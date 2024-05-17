IPCThreadState* IPCThreadState::self()
{
 if (gHaveTLS) {
restart:
 const pthread_key_t k = gTLS;
 IPCThreadState* st = (IPCThreadState*)pthread_getspecific(k);
 if (st) return st;
 return new IPCThreadState;
 }
 
 if (gShutdown) return NULL;
 
    pthread_mutex_lock(&gTLSMutex);
 if (!gHaveTLS) {
 if (pthread_key_create(&gTLS, threadDestructor) != 0) {
            pthread_mutex_unlock(&gTLSMutex);
 return NULL;
 }
        gHaveTLS = true;
 }
    pthread_mutex_unlock(&gTLSMutex);
 goto restart;
}
