void TestingAutomationProvider::LaunchApp(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  std::string id;
  if (!args->GetString("id", &id)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Must include string id.");
    return;
  }

  ExtensionService* service = browser->profile()->GetExtensionService();
  if (!service) {
    AutomationJSONReply(this, reply_message).SendError(
        "No extensions service.");
    return;
  }

  const Extension* extension = service->GetExtensionById(
      id, false   );
  if (!extension) {
    AutomationJSONReply(this, reply_message).SendError(
        StringPrintf("Extension with ID '%s' doesn't exist or is disabled.",
                     id.c_str()));
    return;
  }

  extension_misc::LaunchContainer launch_container =
      service->extension_prefs()->GetLaunchContainer(
          extension, ExtensionPrefs::LAUNCH_REGULAR);

  WebContents* old_contents = browser->GetSelectedWebContents();
  if (!old_contents) {
    AutomationJSONReply(this, reply_message).SendError(
        "Cannot identify selected tab contents.");
    return;
  }

  new AppLaunchObserver(&old_contents->GetController(), this, reply_message,
                        launch_container);
  Browser::OpenApplication(profile(), extension, launch_container, GURL(),
                           CURRENT_TAB);
}
