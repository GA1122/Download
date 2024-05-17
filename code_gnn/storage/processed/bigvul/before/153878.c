BackRenderbuffer::BackRenderbuffer(GLES2DecoderImpl* decoder)
    : decoder_(decoder),
      memory_tracker_(decoder->memory_tracker()),
      bytes_allocated_(0),
      id_(0) {}
