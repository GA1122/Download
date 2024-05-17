bool HpackDecoder::DecodeNextStringLiteral(HpackInputStream* input_stream,
                                           bool is_key,
                                           StringPiece* output) {
  if (input_stream->MatchPrefixAndConsume(kStringLiteralHuffmanEncoded)) {
    string* buffer = is_key ? &key_buffer_ : &value_buffer_;
    bool result = input_stream->DecodeNextHuffmanString(huffman_table_, buffer);
    *output = StringPiece(*buffer);
    return result;
  } else if (input_stream->MatchPrefixAndConsume(
      kStringLiteralIdentityEncoded)) {
    return input_stream->DecodeNextIdentityString(output);
  } else {
    return false;
  }
}
