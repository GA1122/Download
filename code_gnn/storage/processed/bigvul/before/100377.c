void BrowserRenderProcessHost::InitUserScripts() {
  UserScriptMaster* user_script_master = profile()->GetUserScriptMaster();
  DCHECK(user_script_master);

  if (!user_script_master->ScriptsReady()) {
    return;
  }

  SendUserScriptsUpdate(user_script_master->GetSharedMemory());
}
