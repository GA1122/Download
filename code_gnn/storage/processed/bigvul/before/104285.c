 bool RTCPeerConnectionHandlerChromium::addStream(PassRefPtr<MediaStreamDescriptor> mediaStream, PassRefPtr<MediaConstraints> constraints)
 {
     if (!m_webHandler)
        return false;

    return m_webHandler->addStream(mediaStream, constraints);
}
