String XSSAuditor::CanonicalizedSnippetForJavaScript(
    const FilterTokenRequest& request) {
  String string = request.source_tracker.SourceForToken(request.token);
  size_t start_position = 0;
  size_t end_position = string.length();
  size_t found_position = kNotFound;
  size_t last_non_space_position = kNotFound;

  while (start_position < end_position) {
    while (start_position < end_position &&
           IsHTMLSpace<UChar>(string[start_position]))
      start_position++;

    if (request.should_allow_cdata)
      break;

    if (StartsHTMLOpenCommentAt(string, start_position) ||
        StartsSingleLineCommentAt(string, start_position)) {
      while (start_position < end_position &&
             !IsJSNewline(string[start_position]))
        start_position++;
    } else if (StartsMultiLineCommentAt(string, start_position)) {
      if (start_position + 2 < end_position &&
          (found_position = string.Find("*/", start_position + 2)) != kNotFound)
        start_position = found_position + 2;
      else
        start_position = end_position;
    } else
      break;
  }

  String result;
  while (start_position < end_position && !result.length()) {
    last_non_space_position = kNotFound;
    for (found_position = start_position; found_position < end_position;
         found_position++) {
      if (!request.should_allow_cdata) {
        if (StartsSingleLineCommentAt(string, found_position) ||
            StartsMultiLineCommentAt(string, found_position) ||
            StartsHTMLOpenCommentAt(string, found_position) ||
            StartsHTMLCloseCommentAt(string, found_position)) {
          break;
        }
      }
      if (string[found_position] == ',' || string[found_position] == '`')
        break;

      if (last_non_space_position != kNotFound &&
          (StartsOpeningScriptTagAt(string, found_position) ||
           StartsClosingScriptTagAt(string, found_position))) {
        found_position = last_non_space_position + 1;
        break;
      }
      if (found_position > start_position + kMaximumFragmentLengthTarget) {
        if (IsHTMLSpace<UChar>(string[found_position]))
          break;
      }
      if (!IsHTMLSpace<UChar>(string[found_position]))
        last_non_space_position = found_position;
    }
    result = Canonicalize(
        string.Substring(start_position, found_position - start_position),
        kNoTruncation);
    start_position = found_position + 1;
  }

  return result;
}
