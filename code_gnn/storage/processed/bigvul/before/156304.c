void MediaRecorderHandler::OnVideoFrameForTesting(
    const scoped_refptr<media::VideoFrame>& frame,
    const TimeTicks& timestamp) {
  for (const auto& recorder : video_recorders_)
    recorder->OnVideoFrameForTesting(frame, timestamp);
}
