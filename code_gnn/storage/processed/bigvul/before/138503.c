void WorkerThread::didStopRunLoop()
{
    ASSERT(isCurrentThread());
    Platform::current()->didStopWorkerRunLoop();
}
