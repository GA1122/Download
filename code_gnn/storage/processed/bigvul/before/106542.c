String WebPageProxy::getSelectedText()
{
    String text;
    process()->sendSync(Messages::WebPage::GetSelectedText(), Messages::WebPage::GetSelectedText::Reply(text), m_pageID);
    return text;
}
