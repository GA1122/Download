void DrawingBuffer::ClearPlatformLayer() {
  if (layer_)
    layer_->ClearTexture();

  gl_->Flush();
}
