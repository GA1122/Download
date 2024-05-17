JSGlobalContextRef WebFrame::jsContextForWorld(InjectedBundleScriptWorld* world)
{
    return toGlobalRef(m_coreFrame->script()->globalObject(world->coreWorld())->globalExec());
}
