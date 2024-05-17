void ContentSecurityPolicy::reportInvalidPathCharacter(
    const String& directiveName,
    const String& value,
    const char invalidChar) {
  ASSERT(invalidChar == '#' || invalidChar == '?');

  String ignoring =
      "The fragment identifier, including the '#', will be ignored.";
  if (invalidChar == '?')
    ignoring = "The query component, including the '?', will be ignored.";
  String message = "The source list for Content Security Policy directive '" +
                   directiveName +
                   "' contains a source with an invalid path: '" + value +
                   "'. " + ignoring;
  logToConsole(message);
}
