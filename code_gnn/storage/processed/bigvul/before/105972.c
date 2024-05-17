void JSTestEventTargetOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestEventTarget* jsTestEventTarget = jsCast<JSTestEventTarget*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsTestEventTarget->impl(), jsTestEventTarget);
    jsTestEventTarget->releaseImpl();
}
