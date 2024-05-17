void V8Window::openerAttributeSetterCustom(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    DOMWindow* imp = V8Window::toNative(info.Holder());
    ExceptionState es(info.GetIsolate());
    if (!BindingSecurity::shouldAllowAccessToFrame(imp->frame(), es)) {
        es.throwIfNeeded();
        return;
    }

    if (value->IsNull()) {
        ASSERT(imp->frame());
        imp->frame()->loader()->setOpener(0);
    }

    info.Holder()->Delete(name);

    info.This()->Set(name, value);
}
