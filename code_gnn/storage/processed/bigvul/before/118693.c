bool V8WindowShell::installDOMWindow()
{
    DOMWrapperWorld::setInitializingWindow(true);
    DOMWindow* window = m_frame->domWindow();
    v8::Local<v8::Object> windowWrapper = V8ObjectConstructor::newInstance(V8PerContextData::from(m_context.newLocal(m_isolate))->constructorForType(&V8Window::info));
    if (windowWrapper.IsEmpty())
        return false;

    V8Window::installPerContextEnabledProperties(windowWrapper, window, m_isolate);

    V8DOMWrapper::setNativeInfo(v8::Handle<v8::Object>::Cast(windowWrapper->GetPrototype()), &V8Window::info, window);

    v8::Handle<v8::Object> innerGlobalObject = toInnerGlobalObject(m_context.newLocal(m_isolate));
    V8DOMWrapper::setNativeInfo(innerGlobalObject, &V8Window::info, window);
    innerGlobalObject->SetPrototype(windowWrapper);
    V8DOMWrapper::associateObjectWithWrapper<V8Window>(PassRefPtr<DOMWindow>(window), &V8Window::info, windowWrapper, m_isolate, WrapperConfiguration::Dependent);
    DOMWrapperWorld::setInitializingWindow(false);
    return true;
}
