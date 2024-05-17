JSObject* JSFloat64Array::createPrototype(ExecState* exec, JSGlobalObject* globalObject)
{
    return JSFloat64ArrayPrototype::create(exec->globalData(), globalObject, JSFloat64ArrayPrototype::createStructure(exec->globalData(), globalObject, JSArrayBufferViewPrototype::self(exec, globalObject)));
}
