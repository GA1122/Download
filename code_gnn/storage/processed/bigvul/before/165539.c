void ContentSecurityPolicy::ReportInvalidDirectiveValueCharacter(
    const String& directive_name,
    const String& value) {
  String message = "The value for Content Security Policy directive '" +
                   directive_name + "' contains an invalid character: '" +
                   value +
                   "'. Non-whitespace characters outside ASCII 0x21-0x7E must "
                   "be percent-encoded, as described in RFC 3986, section 2.1: "
                   "http://tools.ietf.org/html/rfc3986#section-2.1.";
  LogToConsole(message);
}