JSObject* JSTestMediaQueryListListener::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSTestMediaQueryListListenerPrototype::create(exec->globalData(), globalObject, JSTestMediaQueryListListenerPrototype::createStructure(globalObject->globalData(), globalObject, globalObject->objectPrototype()));
}
