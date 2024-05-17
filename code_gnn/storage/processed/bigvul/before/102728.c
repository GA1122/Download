void CCThreadProxy::setNeedsRedrawOnCCThread()
{
    ASSERT(isImplThread());
    TRACE_EVENT("CCThreadProxy::setNeedsRedrawOnCCThread", this, 0);
    m_schedulerOnCCThread->requestRedraw();
}
