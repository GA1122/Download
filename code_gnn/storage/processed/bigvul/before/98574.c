bool RemoveWindowFunction::RunImpl() {
  int window_id;
  EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &window_id));

  Browser* browser = GetBrowserInProfileWithId(profile(), window_id,
                                               include_incognito(), &error_);
  if (!browser)
    return false;

  browser->CloseWindow();

  return true;
}
