void PluginInfoMessageFilter::Context::DecidePluginStatus(
    const GetPluginInfo_Params& params,
    const WebPluginInfo& plugin,
    const PluginMetadata* plugin_metadata,
    ChromeViewHostMsg_GetPluginInfo_Status* status) const {
#if defined(OS_WIN)
  if (plugin.type == WebPluginInfo::PLUGIN_TYPE_NPAPI &&
      base::win::IsMetroProcess()) {
    status->value =
        ChromeViewHostMsg_GetPluginInfo_Status::kNPAPINotSupported;
    return;
  }
#endif

  ContentSetting plugin_setting = CONTENT_SETTING_DEFAULT;
  bool uses_default_content_setting = true;
  GetPluginContentSetting(plugin, params.top_origin_url, params.url,
                          plugin_metadata->identifier(), &plugin_setting,
                          &uses_default_content_setting);
  DCHECK(plugin_setting != CONTENT_SETTING_DEFAULT);

  PluginMetadata::SecurityStatus plugin_status =
      plugin_metadata->GetSecurityStatus(plugin);
#if defined(ENABLE_PLUGIN_INSTALLATION)
  if (plugin_status == PluginMetadata::SECURITY_STATUS_OUT_OF_DATE &&
      !allow_outdated_plugins_.GetValue()) {
    if (allow_outdated_plugins_.IsManaged()) {
      status->value =
          ChromeViewHostMsg_GetPluginInfo_Status::kOutdatedDisallowed;
    } else {
      status->value = ChromeViewHostMsg_GetPluginInfo_Status::kOutdatedBlocked;
    }
    return;
  }
#endif

  if (plugin_status ==
          PluginMetadata::SECURITY_STATUS_REQUIRES_AUTHORIZATION &&
      plugin.type != WebPluginInfo::PLUGIN_TYPE_PEPPER_IN_PROCESS &&
      plugin.type != WebPluginInfo::PLUGIN_TYPE_PEPPER_OUT_OF_PROCESS &&
      !always_authorize_plugins_.GetValue() &&
      plugin_setting != CONTENT_SETTING_BLOCK &&
      uses_default_content_setting) {
    status->value = ChromeViewHostMsg_GetPluginInfo_Status::kUnauthorized;
    return;
  }

  if (PluginService::GetInstance()->IsPluginUnstable(plugin.path) &&
      !always_authorize_plugins_.GetValue() &&
      plugin_setting != CONTENT_SETTING_BLOCK &&
      uses_default_content_setting) {
    status->value = ChromeViewHostMsg_GetPluginInfo_Status::kUnauthorized;
    return;
  }

  if (plugin_setting == CONTENT_SETTING_ASK)
    status->value = ChromeViewHostMsg_GetPluginInfo_Status::kClickToPlay;
  else if (plugin_setting == CONTENT_SETTING_BLOCK)
    status->value = ChromeViewHostMsg_GetPluginInfo_Status::kBlocked;
}
