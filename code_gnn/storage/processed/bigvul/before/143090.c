OfflineAudioDestinationHandler::OfflineAudioDestinationHandler(
    AudioNode& node,
    unsigned number_of_channels,
    uint32_t frames_to_process,
    float sample_rate)
    : AudioDestinationHandler(node),
      render_target_(nullptr),
      frames_processed_(0),
      frames_to_process_(frames_to_process),
      is_rendering_started_(false),
      number_of_channels_(number_of_channels),
      sample_rate_(sample_rate) {
  channel_count_ = number_of_channels;

  SetInternalChannelCountMode(kExplicit);
  SetInternalChannelInterpretation(AudioBus::kSpeakers);

  if (Context()->GetExecutionContext()) {
    main_thread_task_runner_ = Context()->GetExecutionContext()->GetTaskRunner(
        TaskType::kMiscPlatformAPI);
    DCHECK(main_thread_task_runner_->BelongsToCurrentThread());
  }
}
