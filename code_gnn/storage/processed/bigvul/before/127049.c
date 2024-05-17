void ChromeClientImpl::postAccessibilityNotification(AXObject* obj, AXObjectCache::AXNotification notification)
{
    if (!obj)
        return;

    m_webView->client()->postAccessibilityEvent(WebAXObject(obj), toWebAXEvent(notification));
}
