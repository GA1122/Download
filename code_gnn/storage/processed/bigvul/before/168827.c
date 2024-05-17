ExecuteCodeFunction::InitResult ExecuteCodeInTabFunction::Init() {
  if (init_result_)
    return init_result_.value();

  int tab_id = -1;
  if (args_->GetInteger(0, &tab_id) && tab_id < 0)
    return set_init_result(VALIDATION_FAILURE);

  base::DictionaryValue* details_value = NULL;
  if (!args_->GetDictionary(1, &details_value))
    return set_init_result(VALIDATION_FAILURE);
  std::unique_ptr<InjectDetails> details(new InjectDetails());
  if (!InjectDetails::Populate(*details_value, details.get()))
    return set_init_result(VALIDATION_FAILURE);

  if (tab_id == -1) {
    Browser* browser = chrome_details_.GetCurrentBrowser();
    if (!browser)
      return set_init_result_error(keys::kNoCurrentWindowError);
    content::WebContents* web_contents = NULL;
    if (!ExtensionTabUtil::GetDefaultTab(browser, &web_contents, &tab_id))
      return set_init_result_error(keys::kNoTabInBrowserWindowError);
  }

  execute_tab_id_ = tab_id;
  details_ = std::move(details);
  set_host_id(HostID(HostID::EXTENSIONS, extension()->id()));
  return set_init_result(SUCCESS);
}
