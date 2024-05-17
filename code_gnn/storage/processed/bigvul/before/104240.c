void RTCPeerConnection::didChangeIceState(IceState newState)
{
    ASSERT(scriptExecutionContext()->isContextThread());
    changeIceState(newState);
}
