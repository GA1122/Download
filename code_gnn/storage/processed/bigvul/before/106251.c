void JSTestObj::visitChildren(JSCell* cell, SlotVisitor& visitor)
{
    JSTestObj* thisObject = jsCast<JSTestObj*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    COMPILE_ASSERT(StructureFlags & OverridesVisitChildren, OverridesVisitChildrenWithoutSettingFlag);
    ASSERT(thisObject->structure()->typeInfo().overridesVisitChildren());
    Base::visitChildren(thisObject, visitor);
    if (thisObject->m_cachedAttribute1)
        visitor.append(&thisObject->m_cachedAttribute1);
    if (thisObject->m_cachedAttribute2)
        visitor.append(&thisObject->m_cachedAttribute2);
}
