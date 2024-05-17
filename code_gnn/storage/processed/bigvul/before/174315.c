status_t IPCThreadState::getAndExecuteCommand()
{
 status_t result;
 int32_t cmd;

    result = talkWithDriver();
 if (result >= NO_ERROR) {
 size_t IN = mIn.dataAvail();
 if (IN < sizeof(int32_t)) return result;
        cmd = mIn.readInt32();
        IF_LOG_COMMANDS() {
            alog << "Processing top-level Command: "
 << getReturnString(cmd) << endl;
 }

        pthread_mutex_lock(&mProcess->mThreadCountLock);
        mProcess->mExecutingThreadsCount++;
        pthread_mutex_unlock(&mProcess->mThreadCountLock);

        result = executeCommand(cmd);

        pthread_mutex_lock(&mProcess->mThreadCountLock);
        mProcess->mExecutingThreadsCount--;
        pthread_cond_broadcast(&mProcess->mThreadCountDecrement);
        pthread_mutex_unlock(&mProcess->mThreadCountLock);

        set_sched_policy(mMyThreadId, SP_FOREGROUND);
 }

 return result;
}
