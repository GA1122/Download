void RenderLayerCompositor::enableCompositingMode(bool enable  )
{
    if (enable != m_compositing) {
        m_compositing = enable;

        if (m_compositing) {
            ensureRootLayer();
            notifyIFramesOfCompositingChange();
        } else
            destroyRootLayer();
    }
}
