void OutOfProcessInstance::NavigateTo(const std::string& url,
                                      bool open_in_new_tab) {
  pp::VarDictionary message;
  message.Set(kType, kJSNavigateType);
  message.Set(kJSNavigateUrl, url);
  message.Set(kJSNavigateNewTab, open_in_new_tab);
  PostMessage(message);
}
