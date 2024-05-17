ScreenRecorder::ScreenRecorder(
    MessageLoop* capture_loop,
    MessageLoop* encode_loop,
    base::MessageLoopProxy* network_loop,
    Capturer* capturer,
    Encoder* encoder)
    : capture_loop_(capture_loop),
      encode_loop_(encode_loop),
      network_loop_(network_loop),
      capturer_(capturer),
      encoder_(encoder),
      is_recording_(false),
      network_stopped_(false),
      encoder_stopped_(false),
      max_recordings_(kMaxRecordings),
      recordings_(0),
      frame_skipped_(false),
      sequence_number_(0) {
  DCHECK(capture_loop_);
  DCHECK(encode_loop_);
  DCHECK(network_loop_);
}
