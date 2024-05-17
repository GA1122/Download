JSObject* JSTestInterface::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSTestInterfacePrototype::create(exec->globalData(), globalObject, JSTestInterfacePrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}
