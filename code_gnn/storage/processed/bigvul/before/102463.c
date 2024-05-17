bool LayerTreeCoordinator::forceRepaintAsync(uint64_t callbackID)
{
    ASSERT(!m_forceRepaintAsyncCallbackID);
    m_forceRepaintAsyncCallbackID = callbackID;
    scheduleLayerFlush();
    return true;
}
