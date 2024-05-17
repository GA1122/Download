void InjectedBundlePage::resetAfterTest()
{
    WKBundleFrameRef frame = WKBundlePageGetMainFrame(m_page);
    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContext(frame);
#if PLATFORM(QT)
    DumpRenderTreeSupportQt::resetInternalsObject(context);
#else
    WebCoreTestSupport::resetInternalsObject(context);
#endif
    assignedUrlsCache.clear();
}
