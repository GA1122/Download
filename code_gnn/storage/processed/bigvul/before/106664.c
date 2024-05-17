void WebPageProxy::validateCommand(const String& commandName, PassRefPtr<ValidateCommandCallback> callback)
{
    if (!isValid()) {
        callback->invalidate();
        return;
    }

    uint64_t callbackID = callback->callbackID();
    m_validateCommandCallbacks.set(callbackID, callback.get());
    process()->send(Messages::WebPage::ValidateCommand(commandName, callbackID), m_pageID);
}
