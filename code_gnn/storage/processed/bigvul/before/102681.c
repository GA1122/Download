PassOwnPtr<CCLayerTreeHostImpl> CCLayerTreeHost::createLayerTreeHostImpl()
{
    return CCLayerTreeHostImpl::create(m_settings);
}
