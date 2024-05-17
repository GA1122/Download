void XSSAuditor::SetEncoding(const WTF::TextEncoding& encoding) {
  const size_t kMiniumLengthForSuffixTree = 512;   
  const int kSuffixTreeDepth = 5;

  if (!encoding.IsValid())
    return;

  encoding_ = encoding;

  decoded_url_ = Canonicalize(document_url_.GetString(), kNoTruncation);
  if (decoded_url_.Find(IsRequiredForInjection) == kNotFound)
    decoded_url_ = String();

  if (!http_body_as_string_.IsEmpty()) {
    decoded_http_body_ = Canonicalize(http_body_as_string_, kNoTruncation);
    http_body_as_string_ = String();
    if (decoded_http_body_.Find(IsRequiredForInjection) == kNotFound)
      decoded_http_body_ = String();
    if (decoded_http_body_.length() >= kMiniumLengthForSuffixTree)
      decoded_http_body_suffix_tree_ = WTF::WrapUnique(
          new SuffixTree<ASCIICodebook>(decoded_http_body_, kSuffixTreeDepth));
  }

  if (decoded_url_.IsEmpty() && decoded_http_body_.IsEmpty())
    is_enabled_ = false;
}
