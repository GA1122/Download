void AudioHandler::PullInputs(size_t frames_to_process) {
  DCHECK(Context()->IsAudioThread());

  for (auto& input : inputs_)
    input->Pull(nullptr, frames_to_process);
}
