void Document::updateLayoutIgnorePendingStylesheets(Document::RunPostLayoutTasks runPostLayoutTasks)
{
    DocumentLifecycle::PreventThrottlingScope preventThrottling(lifecycle());

    updateLayoutTreeIgnorePendingStylesheets();
    updateLayout();

    if (runPostLayoutTasks == RunPostLayoutTasksSynchronously && view())
        view()->flushAnyPendingPostLayoutTasks();
}
