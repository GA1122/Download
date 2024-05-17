void WebPageProxy::getSelectionOrContentsAsString(PassRefPtr<StringCallback> prpCallback)
{
    RefPtr<StringCallback> callback = prpCallback;
    if (!isValid()) {
        callback->invalidate();
        return;
    }
    
    uint64_t callbackID = callback->callbackID();
    m_stringCallbacks.set(callbackID, callback.get());
    process()->send(Messages::WebPage::GetSelectionOrContentsAsString(callbackID), m_pageID);
}
