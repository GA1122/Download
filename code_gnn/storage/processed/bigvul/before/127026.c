void ChromeClientImpl::formStateDidChange(const Node* node)
{
    WebFrameImpl* webframe = WebFrameImpl::fromFrame(node->document().frame());
    if (webframe->client())
        webframe->client()->didUpdateCurrentHistoryItem(webframe);
}
