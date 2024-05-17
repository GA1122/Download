void RenderProcessHostImpl::GetAudioOutputControllers(
    const GetAudioOutputControllersCallback& callback) const {
  audio_renderer_host()->GetOutputControllers(callback);
}
