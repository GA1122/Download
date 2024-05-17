void JSTestCustomNamedGetterOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestCustomNamedGetter* jsTestCustomNamedGetter = jsCast<JSTestCustomNamedGetter*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsTestCustomNamedGetter->impl(), jsTestCustomNamedGetter);
    jsTestCustomNamedGetter->releaseImpl();
}
