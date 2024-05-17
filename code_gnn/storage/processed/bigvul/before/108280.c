bool FrameLoader::shouldClose()
{
    Page* page = m_frame->page();
    Chrome* chrome = page ? page->chrome() : 0;
    if (!chrome || !chrome->canRunBeforeUnloadConfirmPanel())
        return true;

    DOMWindow* domWindow = m_frame->existingDOMWindow();
    if (!domWindow)
        return true;

    RefPtr<Document> document = m_frame->document();
    HTMLElement* body = document->body();
    if (!body)
        return true;

    RefPtr<BeforeUnloadEvent> beforeUnloadEvent = BeforeUnloadEvent::create();
    m_pageDismissalEventBeingDispatched = true;
    domWindow->dispatchEvent(beforeUnloadEvent.get(), domWindow->document());
    m_pageDismissalEventBeingDispatched = false;

    if (!beforeUnloadEvent->defaultPrevented())
        document->defaultEventHandler(beforeUnloadEvent.get());
    if (beforeUnloadEvent->result().isNull())
        return true;

    String text = document->displayStringModifiedByEncoding(beforeUnloadEvent->result());
    return chrome->runBeforeUnloadConfirmPanel(text, m_frame);
}
