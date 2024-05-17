void JSTestSerializedScriptValueInterfaceOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestSerializedScriptValueInterface* jsTestSerializedScriptValueInterface = jsCast<JSTestSerializedScriptValueInterface*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsTestSerializedScriptValueInterface->impl(), jsTestSerializedScriptValueInterface);
    jsTestSerializedScriptValueInterface->releaseImpl();
}
