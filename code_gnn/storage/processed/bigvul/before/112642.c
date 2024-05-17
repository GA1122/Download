void DocumentLoader::checkLoadComplete()
{
    if (!m_frame || isLoading())
        return;
#if !ENABLE(THREADED_HTML_PARSER)
    ASSERT(this == frameLoader()->activeDocumentLoader());
#endif
    m_frame->document()->domWindow()->finishedLoading();
}
