void WebPagePrivate::rootLayerCommitTimerFired(Timer<WebPagePrivate>*)
{
    if (m_suspendRootLayerCommit)
        return;

#if DEBUG_AC_COMMIT
    Platform::logAlways(Platform::LogLevelCritical, "%s", WTF_PRETTY_FUNCTION);
#endif

    m_backingStore->d->instrumentBeginFrame();

    requestLayoutIfNeeded();

    if (!compositorDrawsRootLayer() && needsOneShotDrawingSynchronization()) {
#if DEBUG_AC_COMMIT
        Platform::logAlways(Platform::LogLevelCritical,
            "%s: OneShotDrawingSynchronization code path!",
            WTF_PRETTY_FUNCTION);
#endif
        const IntRect windowRect = IntRect(IntPoint::zero(), viewportSize());
        m_backingStore->d->repaint(windowRect, true  , true  );
        return;
    }

    commitRootLayerIfNeeded();
}
