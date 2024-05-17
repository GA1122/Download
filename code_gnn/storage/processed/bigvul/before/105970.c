JSObject* JSTestEventTarget::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSTestEventTargetPrototype::create(exec->globalData(), globalObject, JSTestEventTargetPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}
