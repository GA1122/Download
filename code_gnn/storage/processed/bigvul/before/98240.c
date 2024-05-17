PassRefPtr<ImmutableArray> WebFrame::childFrames()
{
    if (!m_coreFrame)
        return ImmutableArray::create();

    size_t size = m_coreFrame->tree()->childCount();
    if (!size)
        return ImmutableArray::create();

    Vector<RefPtr<APIObject> > vector;
    vector.reserveInitialCapacity(size);

    for (Frame* child = m_coreFrame->tree()->firstChild(); child; child = child->tree()->nextSibling()) {
        WebFrame* webFrame = static_cast<WebFrameLoaderClient*>(child->loader()->client())->webFrame();
        vector.uncheckedAppend(webFrame);
    }

    return ImmutableArray::adopt(vector);
}
