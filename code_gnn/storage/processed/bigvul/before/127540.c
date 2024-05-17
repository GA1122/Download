void LayerWebKitThread::setSuspendedAnimations(const Vector<RefPtr<LayerAnimation> >& animations)
{
    m_suspendedAnimations = animations;
    m_animationsChanged = true;
    setNeedsCommit();
}
