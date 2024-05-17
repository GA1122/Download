void JSSharedWorker::visitChildren(JSCell* cell, SlotVisitor& visitor)
{
    JSSharedWorker* thisObject = jsCast<JSSharedWorker*>(cell);
    ASSERT_GC_OBJECT_INHERITS(thisObject, &s_info);
    COMPILE_ASSERT(StructureFlags & OverridesVisitChildren, OverridesVisitChildrenWithoutSettingFlag);
    ASSERT(thisObject->structure()->typeInfo().overridesVisitChildren());
    Base::visitChildren(thisObject, visitor);

    if (MessagePort* port = thisObject->impl()->port())
        visitor.addOpaqueRoot(port);
}
