Texture::Texture(GLES2DecoderImpl* decoder)
    : decoder_(decoder),
      memory_tracker_(decoder->GetContextGroup()->memory_tracker(),
                      NULL, NULL),
      id_(0) {
}
