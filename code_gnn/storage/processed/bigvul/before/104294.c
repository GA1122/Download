void RTCPeerConnectionHandlerChromium::removeStream(PassRefPtr<MediaStreamDescriptor> mediaStream)
{
    if (!m_webHandler)
        return;

    m_webHandler->removeStream(mediaStream);
}
