void JSTestNamedConstructorConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSTestNamedConstructorPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}
