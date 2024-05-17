void TestingAutomationProvider::GetDownloadsInfo(Browser* browser,
                                                 DictionaryValue* args,
                                                 IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);
  ListValue* list_of_downloads = new ListValue;

  DownloadService* download_service(
      DownloadServiceFactory::GetForProfile(browser->profile()));

  if (download_service->HasCreatedDownloadManager()) {
    std::vector<DownloadItem*> downloads;
    download_service->GetDownloadManager()->
        GetAllDownloads(FilePath(), &downloads);

    for (std::vector<DownloadItem*>::iterator it = downloads.begin();
         it != downloads.end();
         it++) {   
      list_of_downloads->Append(GetDictionaryFromDownloadItem(*it));
    }
  }
  return_value->Set("downloads", list_of_downloads);
  reply.SendSuccess(return_value.get());
}
