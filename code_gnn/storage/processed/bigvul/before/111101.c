IntSize WebPagePrivate::contentsSize() const
{
    if (!m_mainFrame->view())
        return IntSize();

    return m_backingStoreClient->contentsSize();
}
