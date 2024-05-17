void CCThreadProxy::setNeedsRedraw()
{
    ASSERT(isMainThread());
    TRACE_EVENT("CCThreadProxy::setNeedsRedraw", this, 0);
    s_ccThread->postTask(createCCThreadTask(this, &CCThreadProxy::setNeedsRedrawOnCCThread));
}
