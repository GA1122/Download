void JSTestSerializedScriptValueInterface::put(JSCell* cell, ExecState* exec, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    JSTestSerializedScriptValueInterface* thisObject = jsCast<JSTestSerializedScriptValueInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    lookupPut<JSTestSerializedScriptValueInterface, Base>(exec, propertyName, value, &JSTestSerializedScriptValueInterfaceTable, thisObject, slot);
}
