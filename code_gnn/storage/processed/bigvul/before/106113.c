JSValue jsTestObjContentDocument(ExecState* exec, JSValue slotBase, const Identifier&)
{
    JSTestObj* castedThis = jsCast<JSTestObj*>(asObject(slotBase));
    TestObj* impl = static_cast<TestObj*>(castedThis->impl());
    return shouldAllowAccessToNode(exec, impl->contentDocument()) ? toJS(exec, castedThis->globalObject(), WTF::getPtr(impl->contentDocument())) : jsNull();
}
