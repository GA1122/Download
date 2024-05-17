void JSTestInterfaceOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestInterface* jsTestInterface = jsCast<JSTestInterface*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsTestInterface->impl(), jsTestInterface);
    jsTestInterface->releaseImpl();
}
