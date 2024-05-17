void V8Window::showModalDialogMethodCustom(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    LocalDOMWindow* impl = toLocalDOMWindow(V8Window::toImpl(info.Holder()));
    ExceptionState exceptionState(ExceptionState::ExecutionContext, "showModalDialog", "Window", info.Holder(), info.GetIsolate());
    if (!BindingSecurity::shouldAllowAccessToFrame(info.GetIsolate(), impl->frame(), exceptionState)) {
        exceptionState.throwIfNeeded();
        return;
    }

    TOSTRING_VOID(V8StringResource<TreatNullAndUndefinedAsNullString>, urlString, info[0]);
    DialogHandler handler(info[1], ScriptState::current(info.GetIsolate()));
    TOSTRING_VOID(V8StringResource<TreatNullAndUndefinedAsNullString>, dialogFeaturesString, info[2]);

    impl->showModalDialog(urlString, dialogFeaturesString, callingDOMWindow(info.GetIsolate()), enteredDOMWindow(info.GetIsolate()), setUpDialog, &handler);

    v8SetReturnValue(info, handler.returnValue());
}
