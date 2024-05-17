JSObject* JSTestNamedConstructor::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSTestNamedConstructorPrototype::create(exec->globalData(), globalObject, JSTestNamedConstructorPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}
