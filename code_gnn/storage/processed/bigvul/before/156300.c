MediaRecorderHandler::MediaRecorderHandler(
    scoped_refptr<base::SingleThreadTaskRunner> task_runner)
    : video_bits_per_second_(0),
      audio_bits_per_second_(0),
      video_codec_id_(VideoTrackRecorder::CodecId::LAST),
      audio_codec_id_(AudioTrackRecorder::CodecId::LAST),
      recording_(false),
      client_(nullptr),
      task_runner_(std::move(task_runner)),
      weak_factory_(this) {}
