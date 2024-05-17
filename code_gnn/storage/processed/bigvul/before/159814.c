bool XSSAuditor::IsContainedInRequest(const String& decoded_snippet) {
  if (decoded_snippet.IsEmpty())
    return false;
  if (decoded_url_.FindIgnoringCase(decoded_snippet, 0) != kNotFound)
    return true;
  if (decoded_http_body_suffix_tree_ &&
      !decoded_http_body_suffix_tree_->MightContain(decoded_snippet))
    return false;
  return decoded_http_body_.FindIgnoringCase(decoded_snippet, 0) != kNotFound;
}
