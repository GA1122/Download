void RTCPeerConnectionHandlerChromium::stop()
{
    if (!m_webHandler)
        return;

    m_webHandler->stop();
}
