String RTCPeerConnection::readyState() const
{
    switch (m_readyState) {
    case ReadyStateNew:
        return ASCIILiteral("new");
    case ReadyStateOpening:
        return ASCIILiteral("opening");
    case ReadyStateActive:
        return ASCIILiteral("active");
    case ReadyStateClosing:
        return ASCIILiteral("closing");
    case ReadyStateClosed:
        return ASCIILiteral("closed");
    }

    ASSERT_NOT_REACHED();
    return ASCIILiteral("");
}
