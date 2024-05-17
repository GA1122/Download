    static PassOwnPtr<WorkerThreadCancelableTask> create(PassOwnPtr<Closure> closure)
    {
        return adoptPtr(new WorkerThreadCancelableTask(closure));
    }
