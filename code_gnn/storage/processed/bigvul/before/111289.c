void WebPagePrivate::prepareToDestroy()
{
    closeURLRecursively(m_mainFrame);
}
