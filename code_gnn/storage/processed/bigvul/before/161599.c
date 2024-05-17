void AudioHandler::AddInput() {
  inputs_.push_back(AudioNodeInput::Create(*this));
}
