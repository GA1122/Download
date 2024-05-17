void AllocateVideoAndAudioBitrates(ExceptionState& exception_state,
                                   ExecutionContext* context,
                                   const MediaRecorderOptions* options,
                                   MediaStream* stream,
                                   int* audio_bits_per_second,
                                   int* video_bits_per_second) {
  const bool use_video = !stream->getVideoTracks().IsEmpty();
  const bool use_audio = !stream->getAudioTracks().IsEmpty();

  const unsigned kMaxIntAsUnsigned = std::numeric_limits<int>::max();

  int overall_bps = 0;
  if (options->hasBitsPerSecond())
    overall_bps = std::min(options->bitsPerSecond(), kMaxIntAsUnsigned);
  int video_bps = 0;
  if (options->hasVideoBitsPerSecond() && use_video)
    video_bps = std::min(options->videoBitsPerSecond(), kMaxIntAsUnsigned);
  int audio_bps = 0;
  if (options->hasAudioBitsPerSecond() && use_audio)
    audio_bps = std::min(options->audioBitsPerSecond(), kMaxIntAsUnsigned);

  if (use_audio) {
    if (options->hasBitsPerSecond()) {
      if (use_video)
        audio_bps = overall_bps / 10;
      else
        audio_bps = overall_bps;
    }
    if (options->hasAudioBitsPerSecond() || options->hasBitsPerSecond()) {
      if (audio_bps > kLargestAutoAllocatedOpusBitRate) {
        context->AddConsoleMessage(ConsoleMessage::Create(
            kJSMessageSource, kWarningMessageLevel,
            "Clamping calculated audio bitrate (" + String::Number(audio_bps) +
                "bps) to the maximum (" +
                String::Number(kLargestAutoAllocatedOpusBitRate) + "bps)"));
        audio_bps = kLargestAutoAllocatedOpusBitRate;
      }

      if (audio_bps < kSmallestPossibleOpusBitRate) {
        context->AddConsoleMessage(ConsoleMessage::Create(
            kJSMessageSource, kWarningMessageLevel,
            "Clamping calculated audio bitrate (" + String::Number(audio_bps) +
                "bps) to the minimum (" +
                String::Number(kSmallestPossibleOpusBitRate) + "bps)"));
        audio_bps = kSmallestPossibleOpusBitRate;
      }
    } else {
      DCHECK(!audio_bps);
    }
  }

  if (use_video) {
    if (options->hasBitsPerSecond())
      video_bps = overall_bps - audio_bps;
    if (options->hasVideoBitsPerSecond() || options->hasBitsPerSecond()) {
      if (video_bps < kSmallestPossibleVpxBitRate) {
        context->AddConsoleMessage(ConsoleMessage::Create(
            kJSMessageSource, kWarningMessageLevel,
            "Clamping calculated video bitrate (" + String::Number(video_bps) +
                "bps) to the minimum (" +
                String::Number(kSmallestPossibleVpxBitRate) + "bps)"));
        video_bps = kSmallestPossibleVpxBitRate;
      }
    } else {
      DCHECK(!video_bps);
    }
  }

  *video_bits_per_second = video_bps;
  *audio_bits_per_second = audio_bps;
  return;
}
