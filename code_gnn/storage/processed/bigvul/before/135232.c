IntersectionObserverController& Document::ensureIntersectionObserverController()
{
    if (!m_intersectionObserverController)
        m_intersectionObserverController = IntersectionObserverController::create(this);
    return *m_intersectionObserverController;
}
