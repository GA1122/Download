void ChromeClientImpl::contentsSizeChanged(Frame* frame, const IntSize& size) const
{
    m_webView->didChangeContentsSize();

    WebFrameImpl* webframe = WebFrameImpl::fromFrame(frame);
    webframe->didChangeContentsSize(size);
    if (webframe->client())
        webframe->client()->didChangeContentsSize(webframe, size);
}
