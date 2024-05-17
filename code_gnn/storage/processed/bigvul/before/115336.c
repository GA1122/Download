static void didFailProvisionalLoadWithErrorForFrame(WKPageRef page, WKFrameRef frame, WKErrorRef error, WKTypeRef userData, const void* clientInfo)
{
    if (!WKFrameIsMainFrame(frame))
        return;

    const ResourceError& resourceError = toImpl(error)->platformError();
    GOwnPtr<GError> webError(g_error_new_literal(g_quark_from_string(resourceError.domain().utf8().data()),
                                                 resourceError.errorCode(),
                                                 resourceError.localizedDescription().utf8().data()));
    if (resourceError.tlsErrors()) {
        webkitWebViewLoadFailedWithTLSErrors(WEBKIT_WEB_VIEW(clientInfo), resourceError.failingURL().utf8().data(), webError.get(),
            static_cast<GTlsCertificateFlags>(resourceError.tlsErrors()), resourceError.certificate());
    } else
        webkitWebViewLoadFailed(WEBKIT_WEB_VIEW(clientInfo), WEBKIT_LOAD_STARTED, resourceError.failingURL().utf8().data(), webError.get());
}
