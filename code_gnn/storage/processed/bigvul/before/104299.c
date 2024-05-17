bool MockWebRTCPeerConnectionHandler::addICECandidate(const WebRTCICECandidateDescriptor& iceCandidate)
{
    m_client->didGenerateICECandidate(iceCandidate);
    return true;
}
