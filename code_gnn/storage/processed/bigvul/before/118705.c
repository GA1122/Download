bool V8Window::indexedSecurityCheckCustom(v8::Local<v8::Object> host, uint32_t index, v8::AccessType type, v8::Local<v8::Value>)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Handle<v8::Object> window = host->FindInstanceInPrototypeChain(V8Window::GetTemplate(isolate, worldTypeInMainThread(isolate)));
    if (window.IsEmpty())
        return false;

    DOMWindow* targetWindow = V8Window::toNative(window);

    ASSERT(targetWindow);

    Frame* target = targetWindow->frame();
    if (!target)
        return false;

    if (target->loader()->stateMachine()->isDisplayingInitialEmptyDocument())
        target->loader()->didAccessInitialDocument();

    Frame* childFrame =  target->tree().scopedChild(index);

    if (type == v8::ACCESS_HAS && childFrame)
        return true;
    if (type == v8::ACCESS_GET
        && childFrame
        && !host->HasRealIndexedProperty(index)
        && !window->HasRealIndexedProperty(index))
        return true;

    return BindingSecurity::shouldAllowAccessToFrame(target, DoNotReportSecurityError);
}
