bool AudioNode::DisconnectFromOutputIfConnected(unsigned output_index,
                                                AudioParam& param) {
  AudioNodeOutput& output = Handler().Output(output_index);
  if (!output.IsConnectedToAudioParam(param.Handler()))
    return false;
  output.DisconnectAudioParam(param.Handler());
  connected_params_[output_index]->erase(&param);
  return true;
}
