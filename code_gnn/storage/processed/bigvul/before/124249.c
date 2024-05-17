ScreenOrientation::ScreenOrientation(Screen& screen)
    : DOMWindowProperty(screen.frame())
    , m_orientationLockTimer(this, &ScreenOrientation::orientationLockTimerFired)
    , m_lockedOrientations(WebScreenOrientationDefault)
{
}
