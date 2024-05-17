void ChromeClientImpl::show(NavigationPolicy navigationPolicy)
{
    if (!m_webView->client())
        return;

    WebNavigationPolicy policy = static_cast<WebNavigationPolicy>(navigationPolicy);
    if (policy == WebNavigationPolicyIgnore)
        policy = getNavigationPolicy();
    m_webView->client()->show(policy);
}
