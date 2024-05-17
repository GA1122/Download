String XSSAuditor::Canonicalize(String snippet, TruncationKind treatment) {
  String decoded_snippet = FullyDecodeString(snippet, encoding_);

  if (treatment != kNoTruncation) {
    if (decoded_snippet.length() > kMaximumFragmentLengthTarget) {
      size_t position = kMaximumFragmentLengthTarget;
      while (position < decoded_snippet.length() &&
             !IsHTMLSpace(decoded_snippet[position]))
        ++position;
      decoded_snippet.Truncate(position);
    }
    if (treatment == kSrcLikeAttributeTruncation)
      TruncateForSrcLikeAttribute(decoded_snippet);
    else if (treatment == kScriptLikeAttributeTruncation)
      TruncateForScriptLikeAttribute(decoded_snippet);
    else if (treatment == kSemicolonSeparatedScriptLikeAttributeTruncation)
      TruncateForSemicolonSeparatedScriptLikeAttribute(decoded_snippet);
  }

  return decoded_snippet.RemoveCharacters(&IsNonCanonicalCharacter);
}
