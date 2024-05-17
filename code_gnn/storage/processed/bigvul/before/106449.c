void WebPageProxy::clearLoadDependentCallbacks()
{
    Vector<uint64_t> callbackIDsCopy;
    copyToVector(m_loadDependentStringCallbackIDs, callbackIDsCopy);
    m_loadDependentStringCallbackIDs.clear();

    for (size_t i = 0; i < callbackIDsCopy.size(); ++i) {
        RefPtr<StringCallback> callback = m_stringCallbacks.take(callbackIDsCopy[i]);
        if (callback)
            callback->invalidate();
    }
}
