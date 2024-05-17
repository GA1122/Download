void WorkerThread::terminateAndWaitForAllWorkers()
{
    MutexLocker lock(threadSetMutex());
    HashSet<WorkerThread*> threads = workerThreads();
    for (WorkerThread* thread : threads)
        thread->stopInShutdownSequence();

    for (WorkerThread* thread : threads)
        thread->terminationEvent()->wait();
}
