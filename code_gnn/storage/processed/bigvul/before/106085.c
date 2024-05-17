bool JSTestObjOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSTestObj* jsTestObj = jsCast<JSTestObj*>(handle.get().asCell());
    if (!isObservable(jsTestObj))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}
