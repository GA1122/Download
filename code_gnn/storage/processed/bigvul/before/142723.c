inline void FrameLoader::takeObjectSnapshot() const
{
    TRACE_EVENT_OBJECT_SNAPSHOT_WITH_ID("loading", "FrameLoader", this, toTracedValue());
}
