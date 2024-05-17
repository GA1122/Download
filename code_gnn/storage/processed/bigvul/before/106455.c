void WebPageProxy::confirmComposition(const String& compositionString)
{
    process()->send(Messages::WebPage::ConfirmComposition(compositionString), m_pageID);
}
