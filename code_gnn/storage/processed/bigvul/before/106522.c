void WebPageProxy::executeEditCommand(const String& commandName)
{
    if (!isValid())
        return;

    process()->send(Messages::WebPage::ExecuteEditCommand(commandName), m_pageID);
}
