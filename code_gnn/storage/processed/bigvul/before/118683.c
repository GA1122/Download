V8WindowShell::V8WindowShell(Frame* frame, PassRefPtr<DOMWrapperWorld> world, v8::Isolate* isolate)
    : m_frame(frame)
    , m_world(world)
    , m_isolate(isolate)
{
}
