JSObject* JSTestCustomNamedGetter::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSTestCustomNamedGetterPrototype::create(exec->globalData(), globalObject, JSTestCustomNamedGetterPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}
