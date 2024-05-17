void IPCThreadState::shutdown()
{
    gShutdown = true;
 
 if (gHaveTLS) {
 IPCThreadState* st = (IPCThreadState*)pthread_getspecific(gTLS);
 if (st) {
 delete st;
            pthread_setspecific(gTLS, NULL);
 }
        gHaveTLS = false;
 }
}
