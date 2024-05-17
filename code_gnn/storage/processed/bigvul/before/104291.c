void RTCPeerConnectionHandlerChromium::didGenerateICECandidate(const WebKit::WebRTCICECandidateDescriptor& iceCandidate)
{
    m_client->didGenerateIceCandidate(iceCandidate);
}
