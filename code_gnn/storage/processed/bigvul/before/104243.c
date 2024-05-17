void RTCPeerConnection::didRemoveRemoteStream(MediaStreamDescriptor* streamDescriptor)
{
    ASSERT(scriptExecutionContext()->isContextThread());
    ASSERT(streamDescriptor->owner());

    RefPtr<MediaStream> stream = static_cast<MediaStream*>(streamDescriptor->owner());
    stream->streamEnded();

    if (m_readyState == ReadyStateClosed)
        return;

    ASSERT(m_remoteStreams->contains(stream.get()));
    m_remoteStreams->remove(stream.get());

    dispatchEvent(MediaStreamEvent::create(eventNames().removestreamEvent, false, false, stream.release()));
}
