void WebPageProxy::setComposition(const String& compositionString, Vector<WebCore::CompositionUnderline>& underlines, int cursorPosition)
{
    process()->send(Messages::WebPage::SetComposition(compositionString, underlines, cursorPosition), m_pageID);
}
