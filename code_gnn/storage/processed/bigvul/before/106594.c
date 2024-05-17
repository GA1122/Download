void WebPageProxy::reattachToWebProcess()
{
    m_isValid = true;

    context()->relaunchProcessIfNecessary();
    process()->addExistingWebPage(this, m_pageID);

    initializeWebPage();

    m_pageClient->didRelaunchProcess();
}
