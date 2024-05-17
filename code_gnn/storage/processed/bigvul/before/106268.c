bool JSTestSerializedScriptValueInterfaceOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, SlotVisitor& visitor)
{
    JSTestSerializedScriptValueInterface* jsTestSerializedScriptValueInterface = jsCast<JSTestSerializedScriptValueInterface*>(handle.get().asCell());
    if (!isObservable(jsTestSerializedScriptValueInterface))
        return false;
    UNUSED_PARAM(visitor);
    return false;
}
