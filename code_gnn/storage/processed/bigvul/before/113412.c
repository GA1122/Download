void WebProcessProxy::removeMessagePortChannel(uint64_t channelID)
{
    if (!isValid())
        return;

    send(Messages::WebProcess::RemoveMessagePortChannel(channelID),   0);
}
