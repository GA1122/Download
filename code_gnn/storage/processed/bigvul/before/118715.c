void V8Window::showModalDialogMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    DOMWindow* impl = V8Window::toNative(args.Holder());
    ExceptionState es(args.GetIsolate());
    if (!BindingSecurity::shouldAllowAccessToFrame(impl->frame(), es)) {
        es.throwIfNeeded();
        return;
    }

    String urlString = toWebCoreStringWithUndefinedOrNullCheck(args[0]);
    DialogHandler handler(args[1]);
    String dialogFeaturesString = toWebCoreStringWithUndefinedOrNullCheck(args[2]);

    impl->showModalDialog(urlString, dialogFeaturesString, activeDOMWindow(), firstDOMWindow(), setUpDialog, &handler);

    v8SetReturnValue(args, handler.returnValue(args.GetIsolate()));
}
