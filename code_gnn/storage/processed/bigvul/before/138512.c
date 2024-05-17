void WorkerThread::postTask(const WebTraceLocation& location, PassOwnPtr<ExecutionContextTask> task)
{
    backingThread().postTask(location, WorkerThreadTask::create(*this, task, true).leakPtr());
}
