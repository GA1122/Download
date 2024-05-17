void ScriptProcessorHandler::FireProcessEvent(unsigned double_buffer_index) {
  DCHECK(IsMainThread());

  DCHECK_LT(double_buffer_index, 2u);
  if (double_buffer_index > 1)
    return;

  AudioBuffer* input_buffer = input_buffers_[double_buffer_index].Get();
  AudioBuffer* output_buffer = output_buffers_[double_buffer_index].Get();
  DCHECK(output_buffer);
  if (!output_buffer)
    return;

  if (GetNode() && Context() && Context()->GetExecutionContext()) {
    MutexLocker process_locker(process_event_lock_);

    double playback_time = (Context()->CurrentSampleFrame() + buffer_size_) /
                           static_cast<double>(Context()->sampleRate());

    GetNode()->DispatchEvent(AudioProcessingEvent::Create(
        input_buffer, output_buffer, playback_time));
  }
}
