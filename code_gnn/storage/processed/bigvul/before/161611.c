void AudioHandler::DisableOutputsIfNecessary() {
  if (connection_ref_count_ <= 1 && !is_disabled_) {


    if (GetNodeType() != kNodeTypeConvolver &&
        GetNodeType() != kNodeTypeDelay &&
        GetNodeType() != kNodeTypeBiquadFilter &&
        GetNodeType() != kNodeTypeIIRFilter &&
        GetNodeType() != kNodeTypeAnalyser) {
      is_disabled_ = true;
      ClearInternalStateWhenDisabled();
      for (auto& output : outputs_)
        output->Disable();
    }
  }
}
