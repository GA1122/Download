PassOwnPtr<V8WindowShell> V8WindowShell::create(Frame* frame, PassRefPtr<DOMWrapperWorld> world, v8::Isolate* isolate)
{
    return adoptPtr(new V8WindowShell(frame, world, isolate));
}
