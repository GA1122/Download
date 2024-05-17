void CCLayerTreeHost::applyScrollDeltas(const CCScrollUpdateSet& info)
{
    for (size_t i = 0; i < info.size(); ++i) {
        int layerId = info[i].layerId;
        IntSize scrollDelta = info[i].scrollDelta;

        if (rootLayer()->id() == layerId)
            m_client->applyScrollDelta(scrollDelta);
        else
            ASSERT_NOT_REACHED();
    }
}
