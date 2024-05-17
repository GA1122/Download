JSValueRef WebFrame::jsWrapperForWorld(InjectedBundleNodeHandle* nodeHandle, InjectedBundleScriptWorld* world)
{
    JSDOMWindow* globalObject = m_coreFrame->script()->globalObject(world->coreWorld());
    ExecState* exec = globalObject->globalExec();

    JSLock lock(SilenceAssertionsOnly);
    return toRef(exec, toJS(exec, globalObject, nodeHandle->coreNode()));
}
