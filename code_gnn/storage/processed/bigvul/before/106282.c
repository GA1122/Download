void JSTestSerializedScriptValueInterface::visitChildren(JSCell* cell, SlotVisitor& visitor)
{
    JSTestSerializedScriptValueInterface* thisObject = jsCast<JSTestSerializedScriptValueInterface*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    COMPILE_ASSERT(StructureFlags & OverridesVisitChildren, OverridesVisitChildrenWithoutSettingFlag);
    ASSERT(thisObject->structure()->typeInfo().overridesVisitChildren());
    Base::visitChildren(thisObject, visitor);
    if (thisObject->m_cachedValue)
        visitor.append(&thisObject->m_cachedValue);
    if (thisObject->m_cachedReadonlyValue)
        visitor.append(&thisObject->m_cachedReadonlyValue);
}
