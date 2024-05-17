void WebPageProxy::voidCallback(uint64_t callbackID)
{
    RefPtr<VoidCallback> callback = m_voidCallbacks.take(callbackID);
    if (!callback) {
        return;
    }

    callback->performCallback();
}
