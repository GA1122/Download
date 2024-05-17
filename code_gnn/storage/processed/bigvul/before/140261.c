bool V8Window::namedSecurityCheckCustom(v8::Local<v8::Object> host, v8::Local<v8::Value> key, v8::AccessType type, v8::Local<v8::Value>)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Handle<v8::Object> window = V8Window::findInstanceInPrototypeChain(host, isolate);
    if (window.IsEmpty())
        return false;  

    DOMWindow* targetWindow = V8Window::toImpl(window);
    ASSERT(targetWindow);
    if (!targetWindow->isLocalDOMWindow())
        return false;

    LocalFrame* target = toLocalDOMWindow(targetWindow)->frame();
    if (!target)
        return false;

    if (target->loader().stateMachine()->isDisplayingInitialEmptyDocument())
        target->loader().didAccessInitialDocument();

    if (key->IsString()) {
        DEFINE_STATIC_LOCAL(const AtomicString, nameOfProtoProperty, ("__proto__", AtomicString::ConstructFromLiteral));

        AtomicString name = toCoreAtomicString(key.As<v8::String>());
        Frame* childFrame = target->tree().scopedChild(name);
        if (type == v8::ACCESS_HAS && childFrame)
            return true;
        v8::Handle<v8::String> keyString = key.As<v8::String>();
        if (type == v8::ACCESS_GET
            && childFrame
            && !host->HasRealNamedProperty(keyString)
            && !window->HasRealNamedProperty(keyString)
            && name != nameOfProtoProperty)
            return true;
    }

    return BindingSecurity::shouldAllowAccessToFrame(isolate, target, DoNotReportSecurityError);
}
