void WindowSetTimeoutImpl(const v8::FunctionCallbackInfo<v8::Value>& args, bool singleShot, ExceptionState& es)
{
    int argumentCount = args.Length();

    if (argumentCount < 1)
        return;

    DOMWindow* imp = V8Window::toNative(args.Holder());
    ExecutionContext* scriptContext = static_cast<ExecutionContext*>(imp->document());

    if (!scriptContext) {
        es.throwUninformativeAndGenericDOMException(InvalidAccessError);
        return;
    }

    v8::Handle<v8::Value> function = args[0];
    String functionString;
    if (!function->IsFunction()) {
        if (function->IsString()) {
            functionString = toWebCoreString(function.As<v8::String>());
        } else {
            v8::Handle<v8::Value> v8String = function->ToString();

            if (v8String.IsEmpty())
                return;

            functionString = toWebCoreString(v8String);
        }

        if (!functionString.length())
            return;
    }

    if (!BindingSecurity::shouldAllowAccessToFrame(imp->frame(), es))
        return;

    OwnPtr<ScheduledAction> action;
    if (function->IsFunction()) {
        int paramCount = argumentCount >= 2 ? argumentCount - 2 : 0;
        OwnPtr<v8::Local<v8::Value>[]> params;
        if (paramCount > 0) {
            params = adoptArrayPtr(new v8::Local<v8::Value>[paramCount]);
            for (int i = 0; i < paramCount; i++) {
                params[i] = args[i+2];
            }
        }

        ASSERT(imp->frame());
        action = adoptPtr(new ScheduledAction(imp->frame()->script()->currentWorldContext(), v8::Handle<v8::Function>::Cast(function), paramCount, params.get(), args.GetIsolate()));
    } else {
        if (imp->document() && !imp->document()->contentSecurityPolicy()->allowEval()) {
            v8SetReturnValue(args, 0);
            return;
        }
        ASSERT(imp->frame());
        action = adoptPtr(new ScheduledAction(imp->frame()->script()->currentWorldContext(), functionString, KURL(), args.GetIsolate()));
    }

    int32_t timeout = argumentCount >= 2 ? args[1]->Int32Value() : 0;
    int timerId;
    if (singleShot)
        timerId = DOMWindowTimers::setTimeout(imp, action.release(), timeout);
    else
        timerId = DOMWindowTimers::setInterval(imp, action.release(), timeout);

    if (timeout >= 0) {
        double maximumFireInterval = static_cast<double>(timeout) / 1000 / 2;
        V8GCForContextDispose::instance().notifyIdleSooner(maximumFireInterval);
    }

    v8SetReturnValue(args, timerId);
}
