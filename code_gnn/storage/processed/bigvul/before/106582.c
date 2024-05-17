void WebPageProxy::mouseDidMoveOverElement(uint32_t opaqueModifiers, CoreIPC::ArgumentDecoder* arguments)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;

    WebEvent::Modifiers modifiers = static_cast<WebEvent::Modifiers>(opaqueModifiers);

    m_uiClient.mouseDidMoveOverElement(this, modifiers, userData.get());
}
