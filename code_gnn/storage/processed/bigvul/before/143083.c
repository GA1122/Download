void OfflineAudioDestinationHandler::DoOfflineRendering() {
  DCHECK(!IsMainThread());

  unsigned number_of_channels;
  Vector<float*> destinations;
  {
    bool has_lock = ProcessHeap::CrossThreadPersistentMutex().TryLock();
    if (!has_lock) {
      render_thread_task_runner_->PostTask(
          FROM_HERE,
          WTF::Bind(&OfflineAudioDestinationHandler::DoOfflineRendering,
                    WrapRefCounted(this)));
      return;
    }

    number_of_channels = render_target_->numberOfChannels();
    destinations.ReserveInitialCapacity(number_of_channels);
    for (unsigned i = 0; i < number_of_channels; ++i)
      destinations.push_back(render_target_->getChannelData(i).View()->Data());
    ProcessHeap::CrossThreadPersistentMutex().unlock();
  }

  while (frames_to_process_ > 0) {
    if (RenderIfNotSuspended(nullptr, render_bus_.get(),
                             audio_utilities::kRenderQuantumFrames))
      return;

    uint32_t frames_available_to_copy =
        std::min(frames_to_process_, audio_utilities::kRenderQuantumFrames);

    for (unsigned channel_index = 0; channel_index < number_of_channels;
         ++channel_index) {
      const float* source = render_bus_->Channel(channel_index)->Data();
      memcpy(destinations[channel_index] + frames_processed_, source,
             sizeof(float) * frames_available_to_copy);
    }

    frames_processed_ += frames_available_to_copy;

    DCHECK_GE(frames_to_process_, frames_available_to_copy);
    frames_to_process_ -= frames_available_to_copy;
  }

  DCHECK_EQ(frames_to_process_, 0u);
  FinishOfflineRendering();
}
