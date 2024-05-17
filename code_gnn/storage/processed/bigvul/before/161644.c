void AudioHandler::UnsilenceOutputs() {
  for (auto& output : outputs_)
    output->Bus()->ClearSilentFlag();
}
