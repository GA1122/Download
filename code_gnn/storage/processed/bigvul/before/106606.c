void WebPageProxy::runJavaScriptConfirm(uint64_t frameID, const String& message, bool& result)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    process()->responsivenessTimer()->stop();

    result = m_uiClient.runJavaScriptConfirm(this, message, frame);
}
