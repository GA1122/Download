void JSTestMediaQueryListListenerOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestMediaQueryListListener* jsTestMediaQueryListListener = jsCast<JSTestMediaQueryListListener*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsTestMediaQueryListListener->impl(), jsTestMediaQueryListListener);
    jsTestMediaQueryListListener->releaseImpl();
}
