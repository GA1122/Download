bool Extension::CanExecuteScriptEverywhere() const {
  if (location() == Manifest::COMPONENT)
    return true;

  ScriptingWhitelist* whitelist = ExtensionConfig::GetInstance()->whitelist();

  for (ScriptingWhitelist::const_iterator it = whitelist->begin();
       it != whitelist->end(); ++it) {
    if (id() == *it) {
      return true;
    }
  }

  return false;
}
