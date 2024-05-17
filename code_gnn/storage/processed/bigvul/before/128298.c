void FrameView::performPreLayoutTasks()
{
    TRACE_EVENT0("blink", "FrameView::performPreLayoutTasks");
    lifecycle().advanceTo(DocumentLifecycle::InPreLayout);

    TemporaryChange<bool> changeSchedulingEnabled(m_layoutSchedulingEnabled, false);

    if (!m_nestedLayoutCount && !m_inSynchronousPostLayout && m_postLayoutTasksTimer.isActive()) {
        m_inSynchronousPostLayout = true;
        performPostLayoutTasks();
        m_inSynchronousPostLayout = false;
    }

    Document* document = m_frame->document();
    document->notifyResizeForViewportUnits();

    if (!document->styleResolver() || document->styleResolver()->mediaQueryAffectedByViewportChange()) {
        document->styleResolverChanged();
        document->mediaQueryAffectingValueChanged();

        InspectorInstrumentation::mediaQueryResultChanged(document);
    } else {
        document->evaluateMediaQueryList();
    }

    document->updateRenderTreeIfNeeded();
    lifecycle().advanceTo(DocumentLifecycle::StyleClean);
}
