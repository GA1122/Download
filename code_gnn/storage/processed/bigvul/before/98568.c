bool GetWindowFunction::RunImpl() {
  int window_id;
  EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &window_id));

  Browser* browser = GetBrowserInProfileWithId(profile(), window_id,
                                               include_incognito(), &error_);
  if (!browser)
    return false;

  result_.reset(ExtensionTabUtil::CreateWindowValue(browser, false));
  return true;
}
