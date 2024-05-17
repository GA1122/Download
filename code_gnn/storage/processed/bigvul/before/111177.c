const HitTestResult& WebPagePrivate::hitTestResult(const IntPoint& contentPos)
{
    if (m_cachedHitTestContentPos != contentPos) {
        m_cachedHitTestContentPos = contentPos;
        m_cachedHitTestResult = m_mainFrame->eventHandler()->hitTestResultAtPoint(m_cachedHitTestContentPos, true  );
    }

    return m_cachedHitTestResult;
}
