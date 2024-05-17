void FrameLoaderClient::dispatchDidReceiveResponse(WebCore::DocumentLoader*, unsigned long, const ResourceResponse& response)
{
    m_response = response;
}
