void TestingAutomationProvider::CaptureEntirePageJSON(
    DictionaryValue* args,
    IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  WebContents* web_contents;
  std::string error;

  if (!GetTabFromJSONArgs(args, &web_contents, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }

  FilePath::StringType path_str;
  if (!args->GetString("path", &path_str)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'path' missing or invalid");
    return;
  }

  RenderViewHost* render_view = web_contents->GetRenderViewHost();
  if (render_view) {
    FilePath path(path_str);
    PageSnapshotTaker* snapshot_taker = new PageSnapshotTaker(
        this, reply_message,
        TabContentsWrapper::GetCurrentWrapperForContents(web_contents), path);
    snapshot_taker->Start();
  } else {
    AutomationJSONReply(this, reply_message)
        .SendError("Tab has no associated RenderViewHost");
  }
}
