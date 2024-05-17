void FrameLoader::clientRedirected(const KURL& url, double seconds, double fireDate, bool lockBackForwardList)
{
    m_client->dispatchWillPerformClientRedirect(url, seconds, fireDate);
    
    m_sentRedirectNotification = true;
    
    m_quickRedirectComing = (lockBackForwardList || history()->currentItemShouldBeReplaced()) && m_documentLoader && !m_isExecutingJavaScriptFormAction;
}
