void WebContext::UpdateUserScripts() {
  if (!context_.get()) {
    return;
  }

  std::vector<const oxide::UserScript *> scripts;

  for (int i = 0; i < user_scripts_.size(); ++i) {
    UserScript* script = UserScript::FromProxyHandle(user_scripts_.at(i));
    if (!script || script->state() == UserScript::Loading) {
      return;
    } else if (script->state() == UserScript::Loaded) {
      scripts.push_back(script->impl());
    }
  }

  UserScriptMaster::Get(context_.get())
      ->SerializeUserScriptsAndSendUpdates(scripts);
}
