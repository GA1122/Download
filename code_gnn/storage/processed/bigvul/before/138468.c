static void microtaskFunctionCallback(void* data)
{
    OwnPtr<WebThread::Task> task = adoptPtr(static_cast<WebThread::Task*>(data));
    task->run();
}
