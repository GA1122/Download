void Document::didReceiveTask(void* untypedContext)
{
    ASSERT(isMainThread());

    OwnPtr<PerformTaskContext> context = adoptPtr(static_cast<PerformTaskContext*>(untypedContext));
    ASSERT(context);

    Document* document = context->documentReference.get();
    if (!document)
        return;

    Page* page = document->page();
    if ((page && page->defersLoading()) || !document->m_pendingTasks.isEmpty()) {
        document->m_pendingTasks.append(context->task.release());
        return;
    }

    context->task->performTask(document);
}
