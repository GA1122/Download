  bool DecodeHeaderBlock(StringPiece str) {
    return decoder_.HandleControlFrameHeadersData(0, str.data(), str.size()) &&
        decoder_.HandleControlFrameHeadersComplete(0);
  }
