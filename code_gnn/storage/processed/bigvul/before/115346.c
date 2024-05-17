void QtBuiltinBundlePage::didClearWindowForFrame(WKBundleFrameRef frame, WKBundleScriptWorldRef world)
{
    if (!WKBundleFrameIsMainFrame(frame) || WKBundleScriptWorldNormalWorld() != world)
        return;
    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContextForWorld(frame, world);
    registerNavigatorQtObject(context);
}
