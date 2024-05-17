void InjectedBundlePage::didClearWindowForFrame(WKBundleFrameRef frame, WKBundleScriptWorldRef world)
{
    if (!InjectedBundle::shared().isTestRunning())
        return;

    JSGlobalContextRef context = WKBundleFrameGetJavaScriptContextForWorld(frame, world);
    JSObjectRef window = JSContextGetGlobalObject(context);

    if (WKBundleScriptWorldNormalWorld() != world) {
        JSObjectSetProperty(context, window, toJS("__worldID").get(), JSValueMakeNumber(context, TestRunner::worldIDForWorld(world)), kJSPropertyAttributeReadOnly, 0);
        return;
    }

    JSValueRef exception = 0;
    InjectedBundle::shared().testRunner()->makeWindowObject(context, window, &exception);
    InjectedBundle::shared().gcController()->makeWindowObject(context, window, &exception);
    InjectedBundle::shared().eventSendingController()->makeWindowObject(context, window, &exception);
    InjectedBundle::shared().textInputController()->makeWindowObject(context, window, &exception);
    InjectedBundle::shared().accessibilityController()->makeWindowObject(context, window, &exception);

#if PLATFORM(QT)
    DumpRenderTreeSupportQt::injectInternalsObject(context);
#else
    WebCoreTestSupport::injectInternalsObject(context);
#endif
}
