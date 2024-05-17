bool JSTestCustomNamedGetterOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSTestCustomNamedGetter* jsTestCustomNamedGetter = jsCast<JSTestCustomNamedGetter*>(handle.get().asCell());
    if (!isObservable(jsTestCustomNamedGetter))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}
