PassRefPtr<Frame> webkit_web_frame_init_with_web_view(WebKitWebView* webView, HTMLFrameOwnerElement* element)
{
    WebKitWebFrame* frame = WEBKIT_WEB_FRAME(g_object_new(WEBKIT_TYPE_WEB_FRAME, NULL));
    WebKitWebFramePrivate* priv = frame->priv;
    WebKitWebViewPrivate* viewPriv = WEBKIT_WEB_VIEW_GET_PRIVATE(webView);

    priv->webView = webView;
    WebKit::FrameLoaderClient* client = new WebKit::FrameLoaderClient(frame);

    RefPtr<Frame> coreFrame = Frame::create(viewPriv->corePage, element, client);
    priv->coreFrame = coreFrame.get();

    return coreFrame.release();
}
