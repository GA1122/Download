void IPCThreadState::joinThreadPool(bool isMain)
{
    LOG_THREADPOOL("**** THREAD %p (PID %d) IS JOINING THE THREAD POOL\n", (void*)pthread_self(), getpid());

    mOut.writeInt32(isMain ? BC_ENTER_LOOPER : BC_REGISTER_LOOPER);
 
    set_sched_policy(mMyThreadId, SP_FOREGROUND);
 
 status_t result;
 do {
        processPendingDerefs();
        result = getAndExecuteCommand();

 if (result < NO_ERROR && result != TIMED_OUT && result != -ECONNREFUSED && result != -EBADF) {
            ALOGE("getAndExecuteCommand(fd=%d) returned unexpected error %d, aborting",
                  mProcess->mDriverFD, result);
            abort();
 }
 
 if(result == TIMED_OUT && !isMain) {
 break;
 }
 } while (result != -ECONNREFUSED && result != -EBADF);

    LOG_THREADPOOL("**** THREAD %p (PID %d) IS LEAVING THE THREAD POOL err=%p\n",
 (void*)pthread_self(), getpid(), (void*)result);
 
    mOut.writeInt32(BC_EXIT_LOOPER);
    talkWithDriver(false);
}
