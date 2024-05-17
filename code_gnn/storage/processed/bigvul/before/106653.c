void WebPageProxy::stringCallback(const String& resultString, uint64_t callbackID)
{
    RefPtr<StringCallback> callback = m_stringCallbacks.take(callbackID);
    if (!callback) {
        return;
    }

    m_loadDependentStringCallbackIDs.remove(callbackID);

    callback->performCallbackWithReturnValue(resultString.impl());
}
