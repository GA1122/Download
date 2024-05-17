static void WillDispatchTabCreatedEvent(WebContents* contents,
                                        bool active,
                                        Profile* profile,
                                        const Extension* extension,
                                        ListValue* event_args) {
  DictionaryValue* tab_value = ExtensionTabUtil::CreateTabValue(
      contents, extension);
  event_args->Clear();
  event_args->Append(tab_value);
  tab_value->SetBoolean(tab_keys::kSelectedKey, active);
}
