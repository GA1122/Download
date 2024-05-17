bool JSTestEventTargetOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSTestEventTarget* jsTestEventTarget = jsCast<JSTestEventTarget*>(handle.get().asCell());
    if (!isObservable(jsTestEventTarget))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}
