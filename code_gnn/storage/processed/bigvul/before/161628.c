String AudioHandler::NodeTypeName() const {
  switch (node_type_) {
    case kNodeTypeDestination:
      return "AudioDestinationNode";
    case kNodeTypeOscillator:
      return "OscillatorNode";
    case kNodeTypeAudioBufferSource:
      return "AudioBufferSourceNode";
    case kNodeTypeMediaElementAudioSource:
      return "MediaElementAudioSourceNode";
    case kNodeTypeMediaStreamAudioDestination:
      return "MediaStreamAudioDestinationNode";
    case kNodeTypeMediaStreamAudioSource:
      return "MediaStreamAudioSourceNode";
    case kNodeTypeScriptProcessor:
      return "ScriptProcessorNode";
    case kNodeTypeBiquadFilter:
      return "BiquadFilterNode";
    case kNodeTypePanner:
      return "PannerNode";
    case kNodeTypeStereoPanner:
      return "StereoPannerNode";
    case kNodeTypeConvolver:
      return "ConvolverNode";
    case kNodeTypeDelay:
      return "DelayNode";
    case kNodeTypeGain:
      return "GainNode";
    case kNodeTypeChannelSplitter:
      return "ChannelSplitterNode";
    case kNodeTypeChannelMerger:
      return "ChannelMergerNode";
    case kNodeTypeAnalyser:
      return "AnalyserNode";
    case kNodeTypeDynamicsCompressor:
      return "DynamicsCompressorNode";
    case kNodeTypeWaveShaper:
      return "WaveShaperNode";
    case kNodeTypeUnknown:
    case kNodeTypeEnd:
    default:
      NOTREACHED();
      return "UnknownNode";
  }
}
