void OfflineAudioDestinationHandler::InitializeOfflineRenderThread(
    AudioBuffer* render_target) {
  DCHECK(IsMainThread());

  render_target_ = render_target;
  render_bus_ = AudioBus::Create(render_target->numberOfChannels(),
                                 AudioUtilities::kRenderQuantumFrames);
  DCHECK(render_bus_);

  PrepareTaskRunnerForRendering();
}