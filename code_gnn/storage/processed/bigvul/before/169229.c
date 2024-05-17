bool ExecuteWebUIResourceTest(WebContents* web_contents,
                              const std::vector<int>& js_resource_ids) {
  std::vector<int> ids;
  ids.push_back(IDR_WEBUI_JS_WEBUI_RESOURCE_TEST);
  ids.insert(ids.end(), js_resource_ids.begin(), js_resource_ids.end());

  std::string script;
  for (int id : ids) {
    scoped_refptr<base::RefCountedMemory> bytes =
        ui::ResourceBundle::GetSharedInstance().LoadDataResourceBytes(id);

    if (HasGzipHeader(*bytes))
      AppendGzippedResource(*bytes, &script);
    else
      script.append(bytes->front_as<char>(), bytes->size());

    script.append("\n");
  }
  ExecuteScriptAsync(web_contents, script);

  DOMMessageQueue message_queue;
  ExecuteScriptAsync(web_contents, "runTests()");

  std::string message;
  do {
    if (!message_queue.WaitForMessage(&message))
      return false;
  } while (message.compare("\"PENDING\"") == 0);

  return message.compare("\"SUCCESS\"") == 0;
}
