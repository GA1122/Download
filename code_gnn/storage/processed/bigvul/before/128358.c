void FrameView::updateAcceleratedCompositingSettings()
{
    if (RenderView* renderView = this->renderView())
        renderView->compositor()->updateAcceleratedCompositingSettings();
}
