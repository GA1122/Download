JSValue JSTestEventTarget::indexGetter(ExecState* exec, JSValue slotBase, unsigned index)
{
    JSTestEventTarget* thisObj = jsCast<JSTestEventTarget*>(asObject(slotBase));
    ASSERT_GC_OBJECT_INHERITS(thisObj, &s_info);
    return toJS(exec, thisObj->globalObject(), static_cast<TestEventTarget*>(thisObj->impl())->item(index));
}
