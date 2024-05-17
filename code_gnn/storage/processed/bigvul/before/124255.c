void ScreenOrientation::orientationLockTimerFired(Timer<ScreenOrientation>*)
{
    if (m_lockedOrientations == WebScreenOrientationDefault)
        blink::Platform::current()->unlockOrientation();
    else
        blink::Platform::current()->lockOrientation(m_lockedOrientations);
}
