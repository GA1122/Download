WebPlugin* ChromeContentRendererClient::CreatePluginImpl(
      RenderView* render_view,
      WebFrame* frame,
      const WebPluginParams& original_params,
      bool* is_default_plugin) {
  bool found = false;
  *is_default_plugin = false;
  CommandLine* cmd = CommandLine::ForCurrentProcess();
  webkit::npapi::WebPluginInfo info;
  GURL url(original_params.url);
  std::string orig_mime_type = original_params.mimeType.utf8();
  std::string actual_mime_type;
  render_view->Send(new ViewHostMsg_GetPluginInfo(
      render_view->routing_id(), url, frame->top()->document().url(),
      orig_mime_type, &found, &info, &actual_mime_type));

  if (!found)
    return NULL;
  if (!webkit::npapi::IsPluginEnabled(info))
    return NULL;

  *is_default_plugin =
    info.path.value() == webkit::npapi::kDefaultPluginLibraryName;

  if (orig_mime_type == actual_mime_type) {
    UMA_HISTOGRAM_ENUMERATION(kPluginTypeMismatch,
                              PLUGIN_TYPE_MISMATCH_NONE,
                              PLUGIN_TYPE_MISMATCH_NUM_EVENTS);
  } else if (orig_mime_type.empty()) {
    UMA_HISTOGRAM_ENUMERATION(kPluginTypeMismatch,
                              PLUGIN_TYPE_MISMATCH_ORIG_EMPTY,
                              PLUGIN_TYPE_MISMATCH_NUM_EVENTS);
  } else if (orig_mime_type == kApplicationOctetStream) {
    UMA_HISTOGRAM_ENUMERATION(kPluginTypeMismatch,
                              PLUGIN_TYPE_MISMATCH_ORIG_OCTETSTREAM,
                              PLUGIN_TYPE_MISMATCH_NUM_EVENTS);
  } else {
    UMA_HISTOGRAM_ENUMERATION(kPluginTypeMismatch,
                              PLUGIN_TYPE_MISMATCH_ORIG_OTHER,
                              PLUGIN_TYPE_MISMATCH_NUM_EVENTS);
    return NULL;
  }

  const webkit::npapi::PluginGroup* group =
      webkit::npapi::PluginList::Singleton()->GetPluginGroup(info);
  DCHECK(group != NULL);

  ContentSetting plugin_setting = CONTENT_SETTING_DEFAULT;
  std::string resource;
  if (cmd->HasSwitch(switches::kEnableResourceContentSettings))
    resource = group->identifier();
  render_view->Send(new ViewHostMsg_GetPluginContentSetting(
      frame->top()->document().url(), resource, &plugin_setting));
  DCHECK(plugin_setting != CONTENT_SETTING_DEFAULT);

  WebPluginParams params(original_params);
  for (size_t i = 0; i < info.mime_types.size(); ++i) {
    if (info.mime_types[i].mime_type == actual_mime_type) {
      AppendParams(info.mime_types[i].additional_param_names,
                   info.mime_types[i].additional_param_values,
                   &params.attributeNames,
                   &params.attributeValues);
      break;
    }
  }

  ContentSetting outdated_policy = CONTENT_SETTING_ASK;
  ContentSetting authorize_policy = CONTENT_SETTING_ASK;
  if (group->IsVulnerable() || group->RequiresAuthorization()) {
    render_view->Send(new ViewHostMsg_GetPluginPolicies(
        &outdated_policy, &authorize_policy));
  }

  if (group->IsVulnerable()) {
    if (outdated_policy == CONTENT_SETTING_ASK ||
        outdated_policy == CONTENT_SETTING_BLOCK) {
      if (outdated_policy == CONTENT_SETTING_ASK) {
        render_view->Send(new ViewHostMsg_BlockedOutdatedPlugin(
            render_view->routing_id(), group->GetGroupName(),
            GURL(group->GetUpdateURL())));
      }
      return CreatePluginPlaceholder(
          render_view, frame, params, *group, IDR_BLOCKED_PLUGIN_HTML,
          IDS_PLUGIN_OUTDATED, false, outdated_policy == CONTENT_SETTING_ASK);
    } else {
      DCHECK(outdated_policy == CONTENT_SETTING_ALLOW);
    }
  }

  ContentSettingsObserver* observer = ContentSettingsObserver::Get(render_view);
  ContentSetting host_setting =
      observer->GetContentSetting(CONTENT_SETTINGS_TYPE_PLUGINS);

  if (group->RequiresAuthorization() &&
      authorize_policy == CONTENT_SETTING_ASK &&
      (plugin_setting == CONTENT_SETTING_ALLOW ||
       plugin_setting == CONTENT_SETTING_ASK) &&
      host_setting == CONTENT_SETTING_DEFAULT) {
    render_view->Send(new ViewHostMsg_BlockedOutdatedPlugin(
        render_view->routing_id(), group->GetGroupName(), GURL()));
    return CreatePluginPlaceholder(
        render_view, frame, params, *group, IDR_BLOCKED_PLUGIN_HTML,
        IDS_PLUGIN_NOT_AUTHORIZED, false, true);
  }

  if (info.path.value() == webkit::npapi::kDefaultPluginLibraryName ||
      plugin_setting == CONTENT_SETTING_ALLOW ||
      host_setting == CONTENT_SETTING_ALLOW) {
    if (prerender::PrerenderHelper::IsPrerendering(render_view)) {
      return CreatePluginPlaceholder(
          render_view, frame, params, *group, IDR_CLICK_TO_PLAY_PLUGIN_HTML,
          IDS_PLUGIN_LOAD, true, true);
    }

    if (info.name == ASCIIToUTF16(ChromeContentClient::kNaClPluginName)) {
      bool allow_nacl = cmd->HasSwitch(switches::kEnableNaCl);
      if (!allow_nacl) {
        GURL nexe_url;
        if (actual_mime_type == kNaClPluginMimeType) {
          nexe_url = url;   
        } else {
          string16 nacl_attr = ASCIIToUTF16(kNaClPluginManifestAttribute);
          for (size_t i = 0; i < info.mime_types.size(); ++i) {
            if (info.mime_types[i].mime_type == actual_mime_type) {
              const webkit::npapi::WebPluginMimeType& content_type =
                  info.mime_types[i];
              for (size_t i = 0;
                  i < content_type.additional_param_names.size(); ++i) {
                if (content_type.additional_param_names[i] == nacl_attr) {
                  nexe_url = GURL(content_type.additional_param_values[i]);
                  break;
                }
              }
              break;
            }
          }
        }

        const Extension* extension =
            extension_dispatcher_->extensions()->GetByURL(nexe_url);
        allow_nacl = extension &&
            (extension->from_webstore() ||
            extension->location() == Extension::COMPONENT);
      }

      if (!allow_nacl) {
        return CreatePluginPlaceholder(
            render_view, frame, params, *group, IDR_BLOCKED_PLUGIN_HTML,
            IDS_PLUGIN_BLOCKED, false, false);
      }
    }

    bool pepper_plugin_was_registered = false;
    scoped_refptr<webkit::ppapi::PluginModule> pepper_module(
        render_view->pepper_delegate()->CreatePepperPlugin(
            info.path, &pepper_plugin_was_registered));
    if (pepper_plugin_was_registered) {
      if (pepper_module) {
        return render_view->CreatePepperPlugin(
            frame, params, info.path, pepper_module.get());
      }
      return NULL;
    }

    return render_view->CreateNPAPIPlugin(
        frame, params, info.path, actual_mime_type);
  }

  observer->DidBlockContentType(CONTENT_SETTINGS_TYPE_PLUGINS, resource);
  if (plugin_setting == CONTENT_SETTING_ASK) {
    return CreatePluginPlaceholder(
        render_view, frame, params, *group, IDR_CLICK_TO_PLAY_PLUGIN_HTML,
        IDS_PLUGIN_LOAD, false, true);
  } else {
    return CreatePluginPlaceholder(
        render_view, frame, params, *group, IDR_BLOCKED_PLUGIN_HTML,
        IDS_PLUGIN_BLOCKED, false, true);
  }
}
