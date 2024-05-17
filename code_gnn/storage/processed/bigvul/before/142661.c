FrameLoader::FrameLoader(LocalFrame* frame)
    : m_frame(frame)
    , m_progressTracker(ProgressTracker::create(frame))
    , m_loadType(FrameLoadTypeStandard)
    , m_inStopAllLoaders(false)
    , m_checkTimer(this, &FrameLoader::checkTimerFired)
    , m_didAccessInitialDocument(false)
    , m_didAccessInitialDocumentTimer(this, &FrameLoader::didAccessInitialDocumentTimerFired)
    , m_forcedSandboxFlags(SandboxNone)
    , m_dispatchingDidClearWindowObjectInMainWorld(false)
    , m_protectProvisionalLoader(false)
{
    TRACE_EVENT_OBJECT_CREATED_WITH_ID("loading", "FrameLoader", this);
    takeObjectSnapshot();
}
