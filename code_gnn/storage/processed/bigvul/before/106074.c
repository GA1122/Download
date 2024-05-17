void JSTestObjOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    JSTestObj* jsTestObj = jsCast<JSTestObj*>(handle.get().asCell());
    DOMWrapperWorld* world = static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, jsTestObj->impl(), jsTestObj);
    jsTestObj->releaseImpl();
}
