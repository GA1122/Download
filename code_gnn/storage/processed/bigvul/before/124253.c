void ScreenOrientation::lockOrientationAsync(blink::WebScreenOrientations orientations)
{
    if (m_lockedOrientations == orientations)
        return;
    m_lockedOrientations = orientations;
    if (!m_orientationLockTimer.isActive())
        m_orientationLockTimer.startOneShot(0, FROM_HERE);
}
