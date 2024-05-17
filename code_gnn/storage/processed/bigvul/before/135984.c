  void DisablePlugin(Profile* profile) {
    registrar_.Add(this, chrome::NOTIFICATION_PLUGIN_ENABLE_STATUS_CHANGED,
                   content::Source<Profile>(profile));
    scoped_refptr<PluginPrefs> prefs(PluginPrefs::GetForProfile(profile));
    DCHECK(prefs.get());
    prefs->EnablePluginGroup(
        false, base::UTF8ToUTF16(ChromeContentClient::kPDFPluginName));
    disable_run_loop_.Run();
  }
