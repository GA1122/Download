    explicit WorkerThreadCancelableTask(PassOwnPtr<Closure> closure)
        : m_closure(closure)
        , m_weakFactory(this)
        , m_taskCanceled(false)
    { }
