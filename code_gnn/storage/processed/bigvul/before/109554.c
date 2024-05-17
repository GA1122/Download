    PerformTaskContext(WeakPtr<Document> document, PassOwnPtr<ExecutionContextTask> task)
        : documentReference(document)
        , task(task)
    {
    }
