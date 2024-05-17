void LayerWebKitThread::setRunningAnimations(const Vector<RefPtr<LayerAnimation> >& animations)
{
    m_runningAnimations = animations;
    m_animationsChanged = true;
    setNeedsCommit();
}
