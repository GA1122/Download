MediaRecorder* MediaRecorder::Create(ExecutionContext* context,
                                     MediaStream* stream,
                                     const MediaRecorderOptions* options,
                                     ExceptionState& exception_state) {
  MediaRecorder* recorder =
      new MediaRecorder(context, stream, options, exception_state);
  recorder->PauseIfNeeded();

  return recorder;
}
