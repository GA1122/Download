void Document::postTask(PassOwnPtr<Task> task)
{
    callOnMainThread(didReceiveTask, new PerformTaskContext(m_weakFactory.createWeakPtr(), task));
}
