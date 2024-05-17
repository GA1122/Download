bool JSTestNamedConstructorOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSTestNamedConstructor* jsTestNamedConstructor = jsCast<JSTestNamedConstructor*>(handle.get().asCell());
    if (jsTestNamedConstructor->impl()->hasPendingActivity())
        return true;
    if (!isObservable(jsTestNamedConstructor))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}
