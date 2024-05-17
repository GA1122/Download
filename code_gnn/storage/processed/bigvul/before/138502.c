void WorkerThread::didStartRunLoop()
{
    ASSERT(isCurrentThread());
    Platform::current()->didStartWorkerRunLoop();
}
