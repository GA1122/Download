PassRefPtr<PopupMenu> ChromeClientImpl::createPopupMenu(Frame& frame, PopupMenuClient* client) const
{
    if (WebViewImpl::useExternalPopupMenus())
        return adoptRef(new ExternalPopupMenu(frame, client, *m_webView));

    return adoptRef(new PopupMenuChromium(frame, client));
}
