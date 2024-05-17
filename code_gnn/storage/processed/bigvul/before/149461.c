void ContentSecurityPolicy::reportDirectiveAsSourceExpression(
    const String& directiveName,
    const String& sourceExpression) {
  String message = "The Content Security Policy directive '" + directiveName +
                   "' contains '" + sourceExpression +
                   "' as a source expression. Did you mean '" + directiveName +
                   " ...; " + sourceExpression + "...' (note the semicolon)?";
  logToConsole(message);
}
