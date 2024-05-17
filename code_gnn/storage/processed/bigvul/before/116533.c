WebPlugin* ChromeContentRendererClient::CreatePlugin(
    content::RenderView* render_view,
    WebFrame* frame,
    const WebPluginParams& original_params,
    const ChromeViewHostMsg_GetPluginInfo_Status& status,
    const webkit::WebPluginInfo& plugin,
    const std::string& actual_mime_type) {
  ChromeViewHostMsg_GetPluginInfo_Status::Value status_value = status.value;
  GURL url(original_params.url);
  std::string orig_mime_type = original_params.mimeType.utf8();
  PluginPlaceholder* placeholder = NULL;
  if (status_value == ChromeViewHostMsg_GetPluginInfo_Status::kNotFound) {
    MissingPluginReporter::GetInstance()->ReportPluginMissing(
        orig_mime_type, url);
    placeholder = PluginPlaceholder::CreateMissingPlugin(
        render_view, frame, original_params);
  } else {
    scoped_ptr<webkit::npapi::PluginGroup> group(
        webkit::npapi::PluginList::Singleton()->GetPluginGroup(plugin));
    string16 name = group->GetGroupName();

    WebPluginParams params(original_params);
    for (size_t i = 0; i < plugin.mime_types.size(); ++i) {
      if (plugin.mime_types[i].mime_type == actual_mime_type) {
        AppendParams(plugin.mime_types[i].additional_param_names,
                     plugin.mime_types[i].additional_param_values,
                     &params.attributeNames,
                     &params.attributeValues);
        break;
      }
    }
    if (params.mimeType.isNull() && (actual_mime_type.size() > 0)) {
      params.mimeType = WebString::fromUTF8(actual_mime_type.c_str());
    }

    ContentSettingsObserver* observer =
        ContentSettingsObserver::Get(render_view);

    bool is_nacl_plugin =
        plugin.name ==
            ASCIIToUTF16(chrome::ChromeContentClient::kNaClPluginName);
    ContentSettingsType content_type =
        is_nacl_plugin ? CONTENT_SETTINGS_TYPE_JAVASCRIPT :
                         CONTENT_SETTINGS_TYPE_PLUGINS;

    if ((status_value ==
             ChromeViewHostMsg_GetPluginInfo_Status::kUnauthorized ||
         status_value == ChromeViewHostMsg_GetPluginInfo_Status::kClickToPlay ||
         status_value == ChromeViewHostMsg_GetPluginInfo_Status::kBlocked) &&
        observer->plugins_temporarily_allowed()) {
      status_value = ChromeViewHostMsg_GetPluginInfo_Status::kAllowed;
    }

    switch (status_value) {
      case ChromeViewHostMsg_GetPluginInfo_Status::kNotFound: {
        NOTREACHED();
        break;
      }
      case ChromeViewHostMsg_GetPluginInfo_Status::kAllowed: {
        const char* kNaClMimeType = "application/x-nacl";
        bool is_nacl_mime_type = actual_mime_type == kNaClMimeType;
        bool is_nacl_unrestricted;
        if (is_nacl_plugin) {
          is_nacl_unrestricted = CommandLine::ForCurrentProcess()->HasSwitch(
              switches::kEnableNaCl);
        } else {
          is_nacl_unrestricted = true;
        }
        if (is_nacl_plugin || is_nacl_mime_type) {
          GURL manifest_url = is_nacl_mime_type ?
              url : GetNaClContentHandlerURL(actual_mime_type, plugin);
          const Extension* extension =
              extension_dispatcher_->extensions()->GetExtensionOrAppByURL(
                  ExtensionURLInfo(manifest_url));
          bool is_extension_from_webstore =
              extension && extension->from_webstore();
          bool is_extension_unrestricted = extension &&
              (extension->location() == Extension::COMPONENT ||
              extension->location() == Extension::LOAD);
          GURL top_url = frame->top()->document().url();
          if (!IsNaClAllowed(manifest_url,
                             top_url,
                             is_nacl_unrestricted,
                             is_extension_unrestricted,
                             is_extension_from_webstore,
                             &params)) {
            frame->addMessageToConsole(
                WebConsoleMessage(
                    WebConsoleMessage::LevelError,
                    "Only unpacked extensions and apps installed from the "
                    "Chrome Web Store can load NaCl modules without enabling "
                    "Native Client in about:flags."));
            placeholder = PluginPlaceholder::CreateBlockedPlugin(
                render_view, frame, params, plugin, name,
                IDR_BLOCKED_PLUGIN_HTML, IDS_PLUGIN_BLOCKED);
            break;
          }
        }

        if (prerender::PrerenderHelper::IsPrerendering(render_view)) {
          placeholder = PluginPlaceholder::CreateBlockedPlugin(
              render_view, frame, params, plugin, name,
              IDR_CLICK_TO_PLAY_PLUGIN_HTML, IDS_PLUGIN_LOAD);
          placeholder->set_blocked_for_prerendering(true);
          placeholder->set_allow_loading(true);
          break;
        }

        return render_view->CreatePlugin(frame, plugin, params);
      }
      case ChromeViewHostMsg_GetPluginInfo_Status::kDisabled: {
        placeholder = PluginPlaceholder::CreateBlockedPlugin(
            render_view, frame, params, plugin, name,
            IDR_DISABLED_PLUGIN_HTML, IDS_PLUGIN_DISABLED);
        break;
      }
      case ChromeViewHostMsg_GetPluginInfo_Status::kOutdatedBlocked: {
#if defined(ENABLE_PLUGIN_INSTALLATION)
        placeholder = PluginPlaceholder::CreateBlockedPlugin(
            render_view, frame, params, plugin, name,
            IDR_BLOCKED_PLUGIN_HTML, IDS_PLUGIN_OUTDATED);
        placeholder->set_allow_loading(true);
        render_view->Send(new ChromeViewHostMsg_BlockedOutdatedPlugin(
            render_view->GetRoutingID(), placeholder->CreateRoutingId(),
            group->identifier()));
#else
        NOTREACHED();
#endif
        break;
      }
      case ChromeViewHostMsg_GetPluginInfo_Status::kOutdatedDisallowed: {
        placeholder = PluginPlaceholder::CreateBlockedPlugin(
            render_view, frame, params, plugin, name,
            IDR_BLOCKED_PLUGIN_HTML, IDS_PLUGIN_OUTDATED);
        break;
      }
      case ChromeViewHostMsg_GetPluginInfo_Status::kUnauthorized: {
        placeholder = PluginPlaceholder::CreateBlockedPlugin(
            render_view, frame, params, plugin, name,
            IDR_BLOCKED_PLUGIN_HTML, IDS_PLUGIN_NOT_AUTHORIZED);
        placeholder->set_allow_loading(true);
        render_view->Send(new ChromeViewHostMsg_BlockedUnauthorizedPlugin(
            render_view->GetRoutingID(), group->GetGroupName()));
        break;
      }
      case ChromeViewHostMsg_GetPluginInfo_Status::kClickToPlay: {
        placeholder = PluginPlaceholder::CreateBlockedPlugin(
            render_view, frame, params, plugin, name,
            IDR_CLICK_TO_PLAY_PLUGIN_HTML, IDS_PLUGIN_LOAD);
        placeholder->set_allow_loading(true);
        RenderThread::Get()->RecordUserMetrics("Plugin_ClickToPlay");
        observer->DidBlockContentType(content_type, group->identifier());
        break;
      }
      case ChromeViewHostMsg_GetPluginInfo_Status::kBlocked: {
        placeholder = PluginPlaceholder::CreateBlockedPlugin(
            render_view, frame, params, plugin, name,
            IDR_BLOCKED_PLUGIN_HTML, IDS_PLUGIN_BLOCKED);
        placeholder->set_allow_loading(true);
        RenderThread::Get()->RecordUserMetrics("Plugin_Blocked");
        observer->DidBlockContentType(content_type, group->identifier());
        break;
      }
    }
  }
  placeholder->SetStatus(status);
  return placeholder->plugin();
}
