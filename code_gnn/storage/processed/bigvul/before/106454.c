void WebPageProxy::computedPagesCallback(const Vector<WebCore::IntRect>& pageRects, double totalScaleFactorForPrinting, uint64_t callbackID)
{
    RefPtr<ComputedPagesCallback> callback = m_computedPagesCallbacks.take(callbackID);
    if (!callback) {
        return;
    }

    callback->performCallbackWithReturnValue(pageRects, totalScaleFactorForPrinting);
}
