    void ping(CCCompletionEvent* completion)
    {
        hitThreadID = currentThread();
        completion->signal();
    }
