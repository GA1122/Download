void DefaultAudioDestinationHandler::Render(
    AudioBus* destination_bus,
    uint32_t number_of_frames,
    const AudioIOPosition& output_position,
    const AudioIOCallbackMetric& metric) {
  TRACE_EVENT0("webaudio", "DefaultAudioDestinationHandler::Render");

  DenormalDisabler denormal_disabler;

  DCHECK(Context());
  if (!Context()) {
    return;
  }

  Context()->GetDeferredTaskHandler().SetAudioThreadToCurrentThread();

  if (!IsInitialized()) {
    destination_bus->Zero();
    return;
  }

  Context()->HandlePreRenderTasks(output_position, metric);

  AudioBus* rendered_bus = Input(0).Pull(destination_bus, number_of_frames);

  DCHECK(rendered_bus);
  if (!rendered_bus) {
    destination_bus->Zero();
  } else if (rendered_bus != destination_bus) {
    destination_bus->CopyFrom(*rendered_bus);
  }

  Context()->GetDeferredTaskHandler().ProcessAutomaticPullNodes(
      number_of_frames);

  Context()->HandlePostRenderTasks(destination_bus);

  AdvanceCurrentSampleFrame(number_of_frames);

  Context()->UpdateWorkletGlobalScopeOnRenderingThread();
}
