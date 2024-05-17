void AudioHandler::SilenceOutputs() {
  for (auto& output : outputs_)
    output->Bus()->Zero();
}
