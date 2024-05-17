void ContentSecurityPolicy::ReportDirectiveAsSourceExpression(
    const String& directive_name,
    const String& source_expression) {
  String message = "The Content Security Policy directive '" + directive_name +
                   "' contains '" + source_expression +
                   "' as a source expression. Did you mean '" + directive_name +
                   " ...; " + source_expression + "...' (note the semicolon)?";
  LogToConsole(message);
}
