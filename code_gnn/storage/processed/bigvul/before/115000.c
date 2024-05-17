void TestingAutomationProvider::PerformActionOnDownload(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  int id;
  std::string action;

  DownloadService* download_service =
      DownloadServiceFactory::GetForProfile(browser->profile());
  if (!download_service->HasCreatedDownloadManager()) {
    AutomationJSONReply(this, reply_message).SendError("No download manager.");
    return;
  }
  if (!args->GetInteger("id", &id) || !args->GetString("action", &action)) {
    AutomationJSONReply(this, reply_message)
        .SendError("Must include int id and string action.");
    return;
  }

  DownloadManager* download_manager = download_service->GetDownloadManager();
  DownloadItem* selected_item = GetDownloadItemFromId(id, download_manager);
  if (!selected_item) {
    AutomationJSONReply(this, reply_message)
        .SendError(StringPrintf("No download with an id of %d\n", id));
    return;
  }

  if (action == "open") {
    selected_item->AddObserver(
        new AutomationProviderDownloadUpdatedObserver(
            this, reply_message, true));
    selected_item->OpenDownload();
  } else if (action == "toggle_open_files_like_this") {
    DownloadPrefs* prefs =
        DownloadPrefs::FromBrowserContext(selected_item->GetBrowserContext());
    FilePath path = selected_item->GetUserVerifiedFilePath();
    if (!selected_item->ShouldOpenFileBasedOnExtension())
      prefs->EnableAutoOpenBasedOnExtension(path);
    else
      prefs->DisableAutoOpenBasedOnExtension(path);
    AutomationJSONReply(this, reply_message).SendSuccess(NULL);
  } else if (action == "remove") {
    download_manager->AddObserver(
        new AutomationProviderDownloadModelChangedObserver(
            this, reply_message, download_manager));
    selected_item->Remove();
  } else if (action == "decline_dangerous_download") {
    download_manager->AddObserver(
        new AutomationProviderDownloadModelChangedObserver(
            this, reply_message, download_manager));
    selected_item->Delete(DownloadItem::DELETE_DUE_TO_USER_DISCARD);
  } else if (action == "save_dangerous_download") {
    selected_item->AddObserver(new AutomationProviderDownloadUpdatedObserver(
        this, reply_message, false));
    selected_item->DangerousDownloadValidated();
  } else if (action == "toggle_pause") {
    selected_item->AddObserver(new AutomationProviderDownloadUpdatedObserver(
        this, reply_message, false));
    selected_item->TogglePause();
  } else if (action == "cancel") {
    selected_item->AddObserver(new AutomationProviderDownloadUpdatedObserver(
        this, reply_message, false));
    selected_item->Cancel(true);
  } else {
    AutomationJSONReply(this, reply_message)
        .SendError(StringPrintf("Invalid action '%s' given.", action.c_str()));
  }
}
