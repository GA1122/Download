void WebRuntimeFeatures::enableSockets(bool enable)
{
#if ENABLE(WEB_SOCKETS)
    WebSocket::setIsAvailable(enable);
#endif
}
