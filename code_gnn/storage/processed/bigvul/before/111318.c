void WebPagePrivate::scheduleCompositingRun()
{
    if (WebPageCompositorClient* compositorClient = compositor()->client()) {
        double animationTime = compositorClient->requestAnimationFrame();
        compositorClient->invalidate(animationTime);
        return;
    }

    blitVisibleContents();
}
