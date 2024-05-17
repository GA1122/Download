static void TruncateForScriptLikeAttribute(String& decoded_snippet) {
  size_t position = 0;
  if ((position = decoded_snippet.Find("=")) != kNotFound &&
      (position = decoded_snippet.Find(IsNotHTMLSpace<UChar>, position + 1)) !=
          kNotFound &&
      (position = decoded_snippet.Find(
           IsTerminatingCharacter,
           IsHTMLQuote(decoded_snippet[position]) ? position + 1 : position)) !=
          kNotFound) {
    decoded_snippet.Truncate(position);
  }
}
