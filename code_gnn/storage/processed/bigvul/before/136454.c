void ConversionContext::SwitchToChunkState(const PaintChunk& chunk) {
  chunk_to_layer_mapper_.SwitchToChunk(chunk);

  const auto& chunk_state = chunk.properties;
  SwitchToEffect(chunk_state.Effect());
  SwitchToClip(chunk_state.Clip());
  SwitchToTransform(chunk_state.Transform());
}
