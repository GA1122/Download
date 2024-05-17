void CCThreadProxy::start()
{
    ASSERT(isMainThread());
    ASSERT(s_ccThread);
    CCCompletionEvent completion;
    s_ccThread->postTask(createCCThreadTask(this, &CCThreadProxy::initializeImplOnCCThread, AllowCrossThreadAccess(&completion)));
    completion.wait();

    m_started = true;
}
