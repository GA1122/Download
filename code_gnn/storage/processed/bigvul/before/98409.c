WebKitWebFrame* webkit_web_frame_new(WebKitWebView* webView)
{
    g_return_val_if_fail(WEBKIT_IS_WEB_VIEW(webView), NULL);

    WebKitWebFrame* frame = WEBKIT_WEB_FRAME(g_object_new(WEBKIT_TYPE_WEB_FRAME, NULL));
    WebKitWebFramePrivate* priv = frame->priv;
    WebKitWebViewPrivate* viewPriv = WEBKIT_WEB_VIEW_GET_PRIVATE(webView);

    priv->webView = webView;
    WebKit::FrameLoaderClient* client = new WebKit::FrameLoaderClient(frame);
    priv->coreFrame = Frame::create(viewPriv->corePage, 0, client).get();
    priv->coreFrame->init();

    priv->origin = NULL;

    return frame;
}
