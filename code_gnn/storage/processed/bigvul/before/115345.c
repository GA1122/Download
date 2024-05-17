void QtBuiltinBundlePage::didClearWindowForFrame(WKBundlePageRef page, WKBundleFrameRef frame, WKBundleScriptWorldRef world, const void* clientInfo)
{
    static_cast<QtBuiltinBundlePage*>(const_cast<void*>(clientInfo))->didClearWindowForFrame(frame, world);
}
