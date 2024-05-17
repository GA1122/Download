bool OfflineAudioDestinationHandler::RenderIfNotSuspended(
    AudioBus* source_bus,
    AudioBus* destination_bus,
    uint32_t number_of_frames) {
  DenormalDisabler denormal_disabler;

  DCHECK(Context());
  if (!Context())
    return false;

  Context()->GetDeferredTaskHandler().SetAudioThreadToCurrentThread();

  if (!IsInitialized()) {
    destination_bus->Zero();
    return false;
  }

  if (Context()->HandlePreOfflineRenderTasks()) {
    SuspendOfflineRendering();
    return true;
  }

  DCHECK_GE(NumberOfInputs(), 1u);
  if (NumberOfInputs() < 1) {
    destination_bus->Zero();
    return false;
  }
  AudioBus* rendered_bus = Input(0).Pull(destination_bus, number_of_frames);

  if (!rendered_bus) {
    destination_bus->Zero();
  } else if (rendered_bus != destination_bus) {
    destination_bus->CopyFrom(*rendered_bus);
  }

  Context()->GetDeferredTaskHandler().ProcessAutomaticPullNodes(
      number_of_frames);

  Context()->HandlePostOfflineRenderTasks();

  AdvanceCurrentSampleFrame(number_of_frames);

  Context()->UpdateWorkletGlobalScopeOnRenderingThread();

  return false;
}
