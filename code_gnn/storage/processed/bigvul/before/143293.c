void Document::updateStyleAndLayoutIgnorePendingStylesheets(Document::RunPostLayoutTasks runPostLayoutTasks)
{
    updateStyleAndLayoutTreeIgnorePendingStylesheets();
    updateStyleAndLayout();

    if (runPostLayoutTasks == RunPostLayoutTasksSynchronously && view())
        view()->flushAnyPendingPostLayoutTasks();
}
