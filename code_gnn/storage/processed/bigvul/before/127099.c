void AudioRendererAlgorithm::CopyWithAdvance(uint8* dest) {
  CopyWithoutAdvance(dest);
  DropFrame();
}
