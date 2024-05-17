void CCThreadProxy::finishAllRendering()
{
    ASSERT(CCProxy::isMainThread());

    CCCompletionEvent completion;
    s_ccThread->postTask(createCCThreadTask(this, &CCThreadProxy::finishAllRenderingOnCCThread, AllowCrossThreadAccess(&completion)));
    completion.wait();
}
