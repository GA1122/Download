  bool DecodeNextName(HpackInputStream* in, StringPiece* out) {
    return decoder_->DecodeNextName(in, out);
  }
