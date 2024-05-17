String RTCPeerConnection::iceState() const
{
    switch (m_iceState) {
    case IceStateNew:
        return ASCIILiteral("new");
    case IceStateGathering:
        return ASCIILiteral("gathering");
    case IceStateWaiting:
        return ASCIILiteral("waiting");
    case IceStateChecking:
        return ASCIILiteral("checking");
    case IceStateConnected:
        return ASCIILiteral("connected");
    case IceStateCompleted:
        return ASCIILiteral("completed");
    case IceStateFailed:
        return ASCIILiteral("failed");
    case IceStateClosed:
        return ASCIILiteral("closed");
    }

    ASSERT_NOT_REACHED();
    return ASCIILiteral("");
}
