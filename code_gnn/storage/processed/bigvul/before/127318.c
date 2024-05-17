    void didStartProvisionalLoad(WebFrame* frame)
    {
        WebDataSource* ds = frame->provisionalDataSource();
        m_replacesCurrentHistoryItem = ds->replacesCurrentHistoryItem();
        m_frame = frame;
    }
