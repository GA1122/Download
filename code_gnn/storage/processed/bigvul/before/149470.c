void ContentSecurityPolicy::reportInvalidSourceExpression(
    const String& directiveName,
    const String& source) {
  String message = "The source list for Content Security Policy directive '" +
                   directiveName + "' contains an invalid source: '" + source +
                   "'. It will be ignored.";
  if (equalIgnoringCase(source, "'none'"))
    message = message +
              " Note that 'none' has no effect unless it is the only "
              "expression in the source list.";
  logToConsole(message);
}
