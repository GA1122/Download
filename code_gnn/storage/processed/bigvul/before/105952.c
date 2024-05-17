void JSTestCustomNamedGetterConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSTestCustomNamedGetterPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}
