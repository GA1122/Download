bool JSTestMediaQueryListListenerOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSTestMediaQueryListListener* jsTestMediaQueryListListener = jsCast<JSTestMediaQueryListListener*>(handle.get().asCell());
    if (!isObservable(jsTestMediaQueryListListener))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}
