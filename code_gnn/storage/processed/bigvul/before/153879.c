BackTexture::BackTexture(GLES2DecoderImpl* decoder)
    : memory_tracker_(decoder->memory_tracker()),
      bytes_allocated_(0),
      decoder_(decoder) {
  DCHECK(!decoder_->should_use_native_gmb_for_backbuffer_ ||
         decoder_->GetContextGroup()->image_factory());
}
