bool RTCPeerConnectionHandlerChromium::addIceCandidate(PassRefPtr<RTCIceCandidateDescriptor> iceCandidate)
{
    if (!m_webHandler)
        return false;

     return m_webHandler->addICECandidate(iceCandidate);
 }
