void Document::suppressLoadEvent()
{
    if (!loadEventFinished())
        m_loadEventProgress = LoadEventCompleted;
}
