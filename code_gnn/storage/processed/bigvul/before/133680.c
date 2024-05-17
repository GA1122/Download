  HpackDecoderTest()
      : decoder_(ObtainHpackHuffmanTable()),
        decoder_peer_(&decoder_) {}
