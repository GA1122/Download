JSGlobalContextRef WebFrame::jsContext()
{
    return toGlobalRef(m_coreFrame->script()->globalObject(mainThreadNormalWorld())->globalExec());
}
