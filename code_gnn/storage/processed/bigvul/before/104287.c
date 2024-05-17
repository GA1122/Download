void RTCPeerConnectionHandlerChromium::createOffer(PassRefPtr<RTCSessionDescriptionRequest> request, PassRefPtr<MediaConstraints> constraints)
{
    if (!m_webHandler)
        return;

     m_webHandler->createOffer(request, constraints);
 }
