void DocumentLoader::detachFromFrame()
{
    ASSERT(m_frame);
    RefPtr<Frame> protectFrame(m_frame);
    RefPtr<DocumentLoader> protectLoader(this);

    stopLoading();

    m_applicationCacheHost->setDOMApplicationCache(0);
    InspectorInstrumentation::loaderDetachedFromFrame(m_frame, this);
    m_frame = 0;
}
