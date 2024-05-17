void WebPageProxy::dataCallback(const CoreIPC::DataReference& dataReference, uint64_t callbackID)
{
    RefPtr<DataCallback> callback = m_dataCallbacks.take(callbackID);
    if (!callback) {
        return;
    }

    callback->performCallbackWithReturnValue(WebData::create(dataReference.data(), dataReference.size()).get());
}
