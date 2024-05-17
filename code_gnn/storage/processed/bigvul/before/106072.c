JSObject* JSTestObj::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSTestObjPrototype::create(exec->globalData(), globalObject, JSTestObjPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}
