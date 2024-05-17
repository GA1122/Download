void AudioHandler::AddOutput(unsigned number_of_channels) {
  DCHECK(IsMainThread());
  outputs_.push_back(AudioNodeOutput::Create(this, number_of_channels));
  GetNode()->DidAddOutput(NumberOfOutputs());
}
