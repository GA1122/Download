void V8WrapperInstantiationScope::securityCheck(v8::Isolate* isolate, v8::Local<v8::Context> contextForWrapper)
{
    if (m_context.IsEmpty())
        return;
    Frame* frame = toFrameIfNotDetached(contextForWrapper);
    if (!frame)
        return;
    const DOMWrapperWorld& currentWorld = DOMWrapperWorld::world(m_context);
    RELEASE_ASSERT(currentWorld.worldId() == DOMWrapperWorld::world(contextForWrapper).worldId());
    if (currentWorld.isMainWorld()) {
        RELEASE_ASSERT(BindingSecurity::shouldAllowAccessToFrame(isolate, frame, DoNotReportSecurityError));
     }
 }
