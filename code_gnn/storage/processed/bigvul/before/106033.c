void JSTestMediaQueryListListenerConstructor::finishCreation(ExecState* exec, JSDOMGlobalObject* globalObject)
{
    Base::finishCreation(exec->globalData());
    ASSERT(inherits(&s_info));
    putDirect(exec->globalData(), exec->propertyNames().prototype, JSTestMediaQueryListListenerPrototype::self(exec, globalObject), DontDelete | ReadOnly);
}
