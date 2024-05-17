WorkerThread::WorkerThread(PassRefPtr<WorkerLoaderProxy> workerLoaderProxy, WorkerReportingProxy& workerReportingProxy)
    : m_started(false)
    , m_terminated(false)
    , m_shutdown(false)
    , m_workerLoaderProxy(workerLoaderProxy)
    , m_workerReportingProxy(workerReportingProxy)
    , m_webScheduler(nullptr)
    , m_isolate(nullptr)
    , m_shutdownEvent(adoptPtr(Platform::current()->createWaitableEvent()))
    , m_terminationEvent(adoptPtr(Platform::current()->createWaitableEvent()))
{
    MutexLocker lock(threadSetMutex());
    workerThreads().add(this);
}
