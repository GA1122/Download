void CommandBufferProxyImpl::OnUpdateVSyncParameters(base::TimeTicks timebase,
                                                     base::TimeDelta interval) {
  DCHECK(!gl::IsPresentationCallbackEnabled());
  if (!update_vsync_parameters_completion_callback_.is_null())
    update_vsync_parameters_completion_callback_.Run(timebase, interval);
}
