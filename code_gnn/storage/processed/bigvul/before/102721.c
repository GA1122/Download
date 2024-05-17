 void CCThreadProxy::obtainBeginFrameAndCommitTaskFromCCThread(CCCompletionEvent* completion, CCMainThread::Task** taskPtr)
 {
     OwnPtr<CCMainThread::Task> task = createBeginFrameAndCommitTaskOnCCThread();
    *taskPtr = task.leakPtr();
    completion->signal();
}
