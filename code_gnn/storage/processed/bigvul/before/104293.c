bool RTCPeerConnectionHandlerChromium::initialize(PassRefPtr<RTCConfiguration> configuration, PassRefPtr<MediaConstraints> constraints)
{
    m_webHandler = adoptPtr(WebKit::Platform::current()->createRTCPeerConnectionHandler(this));
    return m_webHandler ? m_webHandler->initialize(configuration, constraints) : false;
}
