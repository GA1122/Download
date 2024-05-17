void WebPageProxy::runJavaScriptAlert(uint64_t frameID, const String& message)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    process()->responsivenessTimer()->stop();

    m_uiClient.runJavaScriptAlert(this, message, frame);
}
