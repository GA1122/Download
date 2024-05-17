void TestingAutomationProvider::SetAppLaunchType(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);

  std::string id;
  if (!args->GetString("id", &id)) {
    reply.SendError("Must include string id.");
    return;
  }

  std::string launch_type_str;
  if (!args->GetString("launch_type", &launch_type_str)) {
    reply.SendError("Must specify app launch type.");
    return;
  }

  ExtensionService* service = browser->profile()->GetExtensionService();
  if (!service) {
    reply.SendError("No extensions service.");
    return;
  }

  const Extension* extension = service->GetExtensionById(
      id, true   );
  if (!extension) {
    reply.SendError(
        StringPrintf("Extension with ID '%s' doesn't exist.", id.c_str()));
    return;
  }

  ExtensionPrefs::LaunchType launch_type;
  if (launch_type_str == "pinned") {
    launch_type = ExtensionPrefs::LAUNCH_PINNED;
  } else if (launch_type_str == "regular") {
    launch_type = ExtensionPrefs::LAUNCH_REGULAR;
  } else if (launch_type_str == "fullscreen") {
    launch_type = ExtensionPrefs::LAUNCH_FULLSCREEN;
  } else if (launch_type_str == "window") {
    launch_type = ExtensionPrefs::LAUNCH_WINDOW;
  } else {
    reply.SendError(
        StringPrintf("Unexpected launch type '%s'.", launch_type_str.c_str()));
    return;
  }

  service->extension_prefs()->SetLaunchType(extension->id(), launch_type);
  reply.SendSuccess(NULL);
}
