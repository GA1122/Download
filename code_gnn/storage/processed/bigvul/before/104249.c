PassRefPtr<RTCConfiguration> RTCPeerConnection::parseConfiguration(const Dictionary& configuration, ExceptionCode& ec)
{
    if (configuration.isUndefinedOrNull())
        return 0;

    ArrayValue iceServers;
    bool ok = configuration.get("iceServers", iceServers);
    if (!ok || iceServers.isUndefinedOrNull()) {
        ec = TYPE_MISMATCH_ERR;
        return 0;
    }

    size_t numberOfServers;
    ok = iceServers.length(numberOfServers);
    if (!ok) {
        ec = TYPE_MISMATCH_ERR;
        return 0;
    }

    RefPtr<RTCConfiguration> rtcConfiguration = RTCConfiguration::create();

    for (size_t i = 0; i < numberOfServers; ++i) {
        Dictionary iceServer;
        ok = iceServers.get(i, iceServer);
        if (!ok) {
            ec = TYPE_MISMATCH_ERR;
            return 0;
        }

        String uri, credential;
        ok = iceServer.get("uri", uri);
        if (!ok) {
            ec = TYPE_MISMATCH_ERR;
            return 0;
        }
        KURL url(KURL(), uri);
        if (!url.isValid() || !(url.protocolIs("turn") || url.protocolIs("stun"))) {
            ec = TYPE_MISMATCH_ERR;
            return 0;
        }

        iceServer.get("credential", credential);

        rtcConfiguration->appendServer(RTCIceServer::create(url, credential));
    }

    return rtcConfiguration.release();
}
