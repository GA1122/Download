void ScriptProcessorHandler::FireProcessEventForOfflineAudioContext(
    unsigned double_buffer_index,
    WaitableEvent* waitable_event) {
  DCHECK(IsMainThread());

  DCHECK_LT(double_buffer_index, 2u);
  if (double_buffer_index > 1) {
    waitable_event->Signal();
    return;
  }

  AudioBuffer* input_buffer = input_buffers_[double_buffer_index].Get();
  AudioBuffer* output_buffer = output_buffers_[double_buffer_index].Get();
  DCHECK(output_buffer);
  if (!output_buffer) {
    waitable_event->Signal();
    return;
  }

  if (GetNode() && Context() && Context()->GetExecutionContext()) {
    double playback_time = (Context()->CurrentSampleFrame() + buffer_size_) /
                           static_cast<double>(Context()->sampleRate());
    GetNode()->DispatchEvent(AudioProcessingEvent::Create(
        input_buffer, output_buffer, playback_time));
  }

  waitable_event->Signal();
}
