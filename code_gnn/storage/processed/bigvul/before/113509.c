void WebPagePrivate::scheduleCompositingRun()
{
    if (WebPageCompositorClient* compositorClient = compositor()->client()) {
        double animationTime = compositorClient->requestAnimationFrame();
        compositorClient->invalidate(animationTime);
        return;
    }

    m_backingStore->d->blitVisibleContents();
}
