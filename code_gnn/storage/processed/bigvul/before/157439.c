void MediaElementAudioSourceHandler::Dispose() {
  media_element_->SetAudioSourceNode(nullptr);
  AudioHandler::Dispose();
}
