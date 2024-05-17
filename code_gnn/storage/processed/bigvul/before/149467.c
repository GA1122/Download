void ContentSecurityPolicy::reportInvalidPluginTypes(const String& pluginType) {
  String message;
  if (pluginType.isNull())
    message =
        "'plugin-types' Content Security Policy directive is empty; all "
        "plugins will be blocked.\n";
  else if (pluginType == "'none'")
    message =
        "Invalid plugin type in 'plugin-types' Content Security Policy "
        "directive: '" +
        pluginType +
        "'. Did you mean to set the object-src directive to 'none'?\n";
  else
    message =
        "Invalid plugin type in 'plugin-types' Content Security Policy "
        "directive: '" +
        pluginType + "'.\n";
  logToConsole(message);
}
