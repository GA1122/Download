void WorkerThread::performIdleWork(double deadlineSeconds)
{
    double gcDeadlineSeconds = deadlineSeconds;

    if (m_webScheduler->canExceedIdleDeadlineIfRequired())
        gcDeadlineSeconds = Platform::current()->monotonicallyIncreasingTime() + kLongIdlePeriodSecs;

    if (doIdleGc(gcDeadlineSeconds))
        m_webScheduler->postIdleTaskAfterWakeup(FROM_HERE, WTF::bind<double>(&WorkerThread::performIdleWork, this));
    else
        m_webScheduler->postIdleTask(FROM_HERE, WTF::bind<double>(&WorkerThread::performIdleWork, this));
}
