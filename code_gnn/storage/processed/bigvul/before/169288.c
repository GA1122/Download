void SynchronizeVisualPropertiesMessageFilter::OnSynchronizeVisualProperties(
    const viz::LocalSurfaceId& local_surface_id,
    const viz::FrameSinkId& frame_sink_id,
    const FrameVisualProperties& resize_params) {
  gfx::Rect screen_space_rect_in_dip = resize_params.screen_space_rect;
  if (IsUseZoomForDSFEnabled()) {
    screen_space_rect_in_dip =
        gfx::Rect(gfx::ScaleToFlooredPoint(
                      resize_params.screen_space_rect.origin(),
                      1.f / resize_params.screen_info.device_scale_factor),
                  gfx::ScaleToCeiledSize(
                      resize_params.screen_space_rect.size(),
                      1.f / resize_params.screen_info.device_scale_factor));
  }
  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::BindOnce(
          &SynchronizeVisualPropertiesMessageFilter::OnUpdatedFrameRectOnUI,
          this, screen_space_rect_in_dip));

  content::BrowserThread::PostTask(
      content::BrowserThread::UI, FROM_HERE,
      base::BindOnce(
          &SynchronizeVisualPropertiesMessageFilter::OnUpdatedSurfaceIdOnUI,
          this, local_surface_id));

  frame_sink_id_ = frame_sink_id;

  if (!frame_sink_id_.is_valid())
    return;

  content::BrowserThread::GetTaskRunnerForThread(content::BrowserThread::UI)
      ->PostTask(FROM_HERE,
                 base::BindOnce(&SynchronizeVisualPropertiesMessageFilter::
                                    OnUpdatedFrameSinkIdOnUI,
                                this));
}
