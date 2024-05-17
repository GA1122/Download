void ContentSecurityPolicy::ReportInvalidPluginTypes(
    const String& plugin_type) {
  String message;
  if (plugin_type.IsNull())
    message =
        "'plugin-types' Content Security Policy directive is empty; all "
        "plugins will be blocked.\n";
  else if (plugin_type == "'none'")
    message =
        "Invalid plugin type in 'plugin-types' Content Security Policy "
        "directive: '" +
        plugin_type +
        "'. Did you mean to set the object-src directive to 'none'?\n";
  else
    message =
        "Invalid plugin type in 'plugin-types' Content Security Policy "
        "directive: '" +
        plugin_type + "'.\n";
  LogToConsole(message);
}
