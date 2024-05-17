void WebPageProxy::drawPagesToPDF(WebFrameProxy* frame, uint32_t first, uint32_t count, PassRefPtr<DataCallback> prpCallback)
{
    RefPtr<DataCallback> callback = prpCallback;
    if (!isValid()) {
        callback->invalidate();
        return;
    }
    
    uint64_t callbackID = callback->callbackID();
    m_dataCallbacks.set(callbackID, callback.get());
    process()->send(Messages::WebPage::DrawPagesToPDF(frame->frameID(), first, count, callbackID), m_pageID, m_isPerformingDOMPrintOperation ? CoreIPC::DispatchMessageEvenWhenWaitingForSyncReply : 0);
}
