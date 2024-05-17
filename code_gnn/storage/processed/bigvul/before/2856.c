CheckForShmSyscall(void)
{
    void (*oldHandler) (int);
    int shmid = -1;

     
    oldHandler = OsSignal(SIGSYS, SigSysHandler);

    badSysCall = FALSE;
    shmid = shmget(IPC_PRIVATE, 4096, IPC_CREAT);

    if (shmid != -1) {
         
        shmctl(shmid, IPC_RMID, NULL);
    }
    else {
         
        badSysCall = TRUE;
    }
    OsSignal(SIGSYS, oldHandler);
    return !badSysCall;
}
