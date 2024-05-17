    PerformTaskContext(WeakPtr<Document> document, PassOwnPtr<ScriptExecutionContext::Task> task)
        : documentReference(document)
        , task(task)
    {
    }
