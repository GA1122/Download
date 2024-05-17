void WorkerThread::postDelayedTask(const WebTraceLocation& location, PassOwnPtr<ExecutionContextTask> task, long long delayMs)
{
    backingThread().postDelayedTask(location, WorkerThreadTask::create(*this, task, true).leakPtr(), delayMs);
}
