void WebPageProxy::scriptValueCallback(const CoreIPC::DataReference& dataReference, uint64_t callbackID)
{
    RefPtr<ScriptValueCallback> callback = m_scriptValueCallbacks.take(callbackID);
    if (!callback) {
        return;
    }

    Vector<uint8_t> data;
    data.reserveInitialCapacity(dataReference.size());
    data.append(dataReference.data(), dataReference.size());

    callback->performCallbackWithReturnValue(data.size() ? WebSerializedScriptValue::adopt(data).get() : 0);
}
