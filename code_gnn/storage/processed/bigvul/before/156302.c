void MediaRecorderHandler::OnEncodedVideo(
    const media::WebmMuxer::VideoParameters& params,
    std::unique_ptr<std::string> encoded_data,
    std::unique_ptr<std::string> encoded_alpha,
    TimeTicks timestamp,
    bool is_key_frame) {
  DCHECK(main_render_thread_checker_.CalledOnValidThread());

  if (UpdateTracksAndCheckIfChanged()) {
    client_->OnError("Amount of tracks in MediaStream has changed.");
    return;
  }
  if (!webm_muxer_)
    return;
  if (!webm_muxer_->OnEncodedVideo(params, std::move(encoded_data),
                                   std::move(encoded_alpha), timestamp,
                                   is_key_frame)) {
    DLOG(ERROR) << "Error muxing video data";
    client_->OnError("Error muxing video data");
  }
}
