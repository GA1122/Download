bool WorkerThread::doIdleGc(double deadlineSeconds)
{
    bool gcFinished = false;
    if (deadlineSeconds > Platform::current()->monotonicallyIncreasingTime())
        gcFinished = isolate()->IdleNotificationDeadline(deadlineSeconds);
    return gcFinished;
}
