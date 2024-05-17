void ExtensionService::NotifyExtensionLoaded(const Extension* extension) {
  profile_->RegisterExtensionWithRequestContexts(extension);

  NotificationService::current()->Notify(
      NotificationType::EXTENSION_LOADED,
      Source<Profile>(profile_),
      Details<const Extension>(extension));

  for (RenderProcessHost::iterator i(RenderProcessHost::AllHostsIterator());
       !i.IsAtEnd(); i.Advance()) {
    RenderProcessHost* host = i.GetCurrentValue();
    if (host->profile()->GetOriginalProfile() ==
        profile_->GetOriginalProfile()) {
      host->Send(
          new ExtensionMsg_Loaded(ExtensionMsg_Loaded_Params(extension)));
    }
  }


  profile_->GetExtensionSpecialStoragePolicy()->
      GrantRightsForExtension(extension);

  UpdateActiveExtensionsInCrashReporter();

  ExtensionWebUI::RegisterChromeURLOverrides(
      profile_, extension->GetChromeURLOverrides());

  if (profile_->GetTemplateURLModel())
    profile_->GetTemplateURLModel()->RegisterExtensionKeyword(extension);

  if (!extension->omnibox_keyword().empty()) {
    omnibox_popup_icon_manager_.LoadIcon(extension);
    omnibox_icon_manager_.LoadIcon(extension);
  }

  if (extension->HasHostPermission(GURL(chrome::kChromeUIFaviconURL))) {
    FaviconSource* favicon_source = new FaviconSource(profile_,
                                                      FaviconSource::FAVICON);
    profile_->GetChromeURLDataManager()->AddDataSource(favicon_source);
  }

  bool plugins_changed = false;
  for (size_t i = 0; i < extension->plugins().size(); ++i) {
    const Extension::PluginInfo& plugin = extension->plugins()[i];
    webkit::npapi::PluginList::Singleton()->RefreshPlugins();
    webkit::npapi::PluginList::Singleton()->AddExtraPluginPath(plugin.path);
    plugins_changed = true;
    if (!plugin.is_public) {
      PluginService::GetInstance()->RestrictPluginToUrl(
          plugin.path, extension->url());
    }
  }

  bool nacl_modules_changed = false;
  for (size_t i = 0; i < extension->nacl_modules().size(); ++i) {
    const Extension::NaClModuleInfo& module = extension->nacl_modules()[i];
    RegisterNaClModule(module.url, module.mime_type);
    nacl_modules_changed = true;
  }

  if (nacl_modules_changed)
    UpdatePluginListWithNaClModules();

  if (plugins_changed || nacl_modules_changed)
    PluginService::GetInstance()->PurgePluginListCache(false);
}
