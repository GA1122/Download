DictionaryValue* ExtensionSettingsHandler::CreateExtensionDetailValue(
    ExtensionService* service, const Extension* extension,
    const std::vector<ExtensionPage>& pages,
    const ExtensionWarningSet* warnings_set,
    bool enabled, bool terminated) {
  DictionaryValue* extension_data = new DictionaryValue();
  GURL icon =
      ExtensionIconSource::GetIconURL(extension,
                                      Extension::EXTENSION_ICON_MEDIUM,
                                      ExtensionIconSet::MATCH_BIGGER,
                                      !enabled, NULL);
  extension_data->SetString("id", extension->id());
  extension_data->SetString("name", extension->name());
  extension_data->SetString("description", extension->description());
  if (extension->location() == Extension::LOAD)
    extension_data->SetString("path", extension->path().value());
  extension_data->SetString("version", extension->version()->GetString());
  extension_data->SetString("icon", icon.spec());
  extension_data->SetBoolean("isUnpacked",
                             extension->location() == Extension::LOAD);
  extension_data->SetBoolean("mayDisable",
                             Extension::UserMayDisable(extension->location()));
  extension_data->SetBoolean("enabled", enabled);
  extension_data->SetBoolean("terminated", terminated);
  extension_data->SetBoolean("enabledIncognito",
      service ? service->IsIncognitoEnabled(extension->id()) : false);
  extension_data->SetBoolean("wantsFileAccess", extension->wants_file_access());
  extension_data->SetBoolean("allowFileAccess",
      service ? service->AllowFileAccess(extension) : false);
  extension_data->SetBoolean("allow_reload",
                             extension->location() == Extension::LOAD);
  extension_data->SetBoolean("is_hosted_app", extension->is_hosted_app());

  if (extension->location() == Extension::LOAD)
    extension_data->SetInteger("order", 1);
  else
    extension_data->SetInteger("order", 2);

  if (!extension->options_url().is_empty() && enabled)
    extension_data->SetString("options_url", extension->options_url().spec());

  if (service && !service->GetBrowserActionVisibility(extension))
    extension_data->SetBoolean("enable_show_button", true);

  ListValue* views = new ListValue;
  for (std::vector<ExtensionPage>::const_iterator iter = pages.begin();
       iter != pages.end(); ++iter) {
    DictionaryValue* view_value = new DictionaryValue;
    if (iter->url.scheme() == chrome::kExtensionScheme) {
      view_value->SetString("path", iter->url.path().substr(1));
    } else {
      view_value->SetString("path", iter->url.spec());
    }
    view_value->SetInteger("renderViewId", iter->render_view_id);
    view_value->SetInteger("renderProcessId", iter->render_process_id);
    view_value->SetBoolean("incognito", iter->incognito);
    views->Append(view_value);
  }
  extension_data->Set("views", views);
  extension_data->SetBoolean("hasPopupAction",
      extension->browser_action() || extension->page_action());
  extension_data->SetString("homepageUrl", extension->GetHomepageURL().spec());

  ListValue* warnings_list = new ListValue;
  if (warnings_set) {
    std::set<ExtensionWarningSet::WarningType> warnings;
    warnings_set->GetWarningsAffectingExtension(extension->id(), &warnings);

    for (std::set<ExtensionWarningSet::WarningType>::const_iterator iter =
             warnings.begin();
         iter != warnings.end();
         ++iter) {
      string16 warning_string(ExtensionWarningSet::GetLocalizedWarning(*iter));
      warnings_list->Append(Value::CreateStringValue(warning_string));
    }
  }
  extension_data->Set("warnings", warnings_list);

  return extension_data;
}
