void ContentSecurityPolicy::ReportInvalidPathCharacter(
    const String& directive_name,
    const String& value,
    const char invalid_char) {
  DCHECK(invalid_char == '#' || invalid_char == '?');

  String ignoring =
      "The fragment identifier, including the '#', will be ignored.";
  if (invalid_char == '?')
    ignoring = "The query component, including the '?', will be ignored.";
  String message = "The source list for Content Security Policy directive '" +
                   directive_name +
                   "' contains a source with an invalid path: '" + value +
                   "'. " + ignoring;
  LogToConsole(message);
}
