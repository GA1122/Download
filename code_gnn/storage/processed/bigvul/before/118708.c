void V8Window::openMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    DOMWindow* impl = V8Window::toNative(args.Holder());
    ExceptionState es(args.GetIsolate());
    if (!BindingSecurity::shouldAllowAccessToFrame(impl->frame(), es)) {
        es.throwIfNeeded();
        return;
    }

    String urlString = toWebCoreStringWithUndefinedOrNullCheck(args[0]);
    AtomicString frameName = (args[1]->IsUndefined() || args[1]->IsNull()) ? "_blank" : toWebCoreAtomicString(args[1]);
    String windowFeaturesString = toWebCoreStringWithUndefinedOrNullCheck(args[2]);

    RefPtr<DOMWindow> openedWindow = impl->open(urlString, frameName, windowFeaturesString, activeDOMWindow(), firstDOMWindow());
    if (!openedWindow)
        return;

    v8SetReturnValueFast(args, openedWindow.release(), impl);
}
