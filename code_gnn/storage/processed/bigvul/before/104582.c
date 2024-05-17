bool Extension::CanExecuteScriptEverywhere() const {
  if (location() == Extension::COMPONENT
#ifndef NDEBUG
      || CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kExposePrivateExtensionApi)
#endif
      )
    return true;

  ScriptingWhitelist* whitelist =
      ExtensionConfig::GetInstance()->whitelist();

  for (ScriptingWhitelist::const_iterator it = whitelist->begin();
       it != whitelist->end(); ++it) {
    if (id() == *it) {
      return true;
    }
  }

  return false;
}
