void TestingAutomationProvider::DragAndDropFilePaths(
    DictionaryValue* args, IPC::Message* reply_message) {
  if (SendErrorIfModalDialogActive(this, reply_message))
    return;

  RenderViewHost* view;
  std::string error;
  if (!GetRenderViewFromJSONArgs(args, profile(), &view, &error)) {
    AutomationJSONReply(this, reply_message).SendError(error);
    return;
  }

  int x, y;
  if (!args->GetInteger("x", &x) || !args->GetInteger("y", &y)) {
    AutomationJSONReply(this, reply_message)
        .SendError("(X,Y) coordinates missing or invalid");
    return;
  }

  ListValue* paths = NULL;
  if (!args->GetList("paths", &paths)) {
    AutomationJSONReply(this, reply_message)
        .SendError("'paths' missing or invalid");
    return;
  }

  WebDropData drop_data;
  for (size_t path_index = 0; path_index < paths->GetSize(); ++path_index) {
    string16 path;
    if (!paths->GetString(path_index, &path)) {
      AutomationJSONReply(this, reply_message)
          .SendError("'paths' contains a non-string type");
      return;
    }

    drop_data.filenames.push_back(path);
  }

  const gfx::Point client(x, y);
  const gfx::Point screen;

  int operations = 0;
  operations |= WebKit::WebDragOperationCopy;
  operations |= WebKit::WebDragOperationLink;
  operations |= WebKit::WebDragOperationMove;

  view->DragTargetDragEnter(
      drop_data, client, screen,
      static_cast<WebKit::WebDragOperationsMask>(operations));
  new DragTargetDropAckNotificationObserver(this, reply_message);
  view->DragTargetDrop(client, screen);
}
