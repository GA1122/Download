void Document::documentDidResumeFromPageCache() 
{
    Vector<Element*> elements;
    copyToVector(m_documentSuspensionCallbackElements, elements);
    Vector<Element*>::iterator end = elements.end();
    for (Vector<Element*>::iterator i = elements.begin(); i != end; ++i)
        (*i)->documentDidResumeFromPageCache();

#if USE(ACCELERATED_COMPOSITING)
    if (renderer())
        renderView()->setIsInWindow(true);
#endif

    if (FrameView* frameView = view())
        frameView->setAnimatorsAreActive();

    ASSERT(m_frame);
    m_frame->loader()->client()->dispatchDidBecomeFrameset(isFrameSet());
}
