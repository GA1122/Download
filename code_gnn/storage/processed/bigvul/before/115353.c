QtBuiltinBundlePage::~QtBuiltinBundlePage()
{
    if (!m_navigatorQtObject)
        return;
    WKBundleFrameRef frame = WKBundlePageGetMainFrame(m_page);
    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContext(frame);
    JSValueUnprotect(context, m_navigatorQtObject);
}
