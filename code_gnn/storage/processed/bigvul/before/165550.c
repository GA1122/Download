void ContentSecurityPolicy::ReportUnsupportedDirective(const String& name) {
  static const char kAllow[] = "allow";
  static const char kOptions[] = "options";
  static const char kPolicyURI[] = "policy-uri";
  static const char kAllowMessage[] =
      "The 'allow' directive has been replaced with 'default-src'. Please use "
      "that directive instead, as 'allow' has no effect.";
  static const char kOptionsMessage[] =
      "The 'options' directive has been replaced with 'unsafe-inline' and "
      "'unsafe-eval' source expressions for the 'script-src' and 'style-src' "
      "directives. Please use those directives instead, as 'options' has no "
      "effect.";
  static const char kPolicyURIMessage[] =
      "The 'policy-uri' directive has been removed from the "
      "specification. Please specify a complete policy via "
      "the Content-Security-Policy header.";

  String message =
      "Unrecognized Content-Security-Policy directive '" + name + "'.\n";
  MessageLevel level = kErrorMessageLevel;
  if (EqualIgnoringASCIICase(name, kAllow)) {
    message = kAllowMessage;
  } else if (EqualIgnoringASCIICase(name, kOptions)) {
    message = kOptionsMessage;
  } else if (EqualIgnoringASCIICase(name, kPolicyURI)) {
    message = kPolicyURIMessage;
  } else if (GetDirectiveType(name) != DirectiveType::kUndefined) {
    message = "The Content-Security-Policy directive '" + name +
              "' is implemented behind a flag which is currently disabled.\n";
    level = kInfoMessageLevel;
  }

  LogToConsole(message, level);
}
