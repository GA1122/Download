void WebPageProxy::runJavaScriptInMainFrame(const String& script, PassRefPtr<ScriptValueCallback> prpCallback)
{
    RefPtr<ScriptValueCallback> callback = prpCallback;
    if (!isValid()) {
        callback->invalidate();
        return;
    }

    uint64_t callbackID = callback->callbackID();
    m_scriptValueCallbacks.set(callbackID, callback.get());
    process()->send(Messages::WebPage::RunJavaScriptInMainFrame(script, callbackID), m_pageID);
}
