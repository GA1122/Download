bool Document::dispatchBeforeUnloadEvent(Chrome& chrome, Document* navigatingDocument)
{
    if (!m_domWindow)
        return true;

    if (!body())
        return true;

    RefPtr<Document> protect(this);

    RefPtr<BeforeUnloadEvent> beforeUnloadEvent = BeforeUnloadEvent::create();
    m_loadEventProgress = BeforeUnloadEventInProgress;
    dispatchWindowEvent(beforeUnloadEvent.get(), this);
    m_loadEventProgress = BeforeUnloadEventCompleted;
    if (!beforeUnloadEvent->defaultPrevented())
        defaultEventHandler(beforeUnloadEvent.get());
    if (beforeUnloadEvent->returnValue().isNull())
        return true;

    if (navigatingDocument->m_didAllowNavigationViaBeforeUnloadConfirmationPanel) {
        addConsoleMessage(JSMessageSource, ErrorMessageLevel, "Blocked attempt to show multiple 'beforeunload' confirmation panels for a single navigation.");
        return true;
    }

    String text = beforeUnloadEvent->returnValue();
    if (chrome.runBeforeUnloadConfirmPanel(text, m_frame)) {
        navigatingDocument->m_didAllowNavigationViaBeforeUnloadConfirmationPanel = true;
        return true;
    }
    return false;
}
