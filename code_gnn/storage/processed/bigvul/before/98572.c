bool CreateWindowFunction::RunImpl() {
  GURL url;
  DictionaryValue* args = NULL;

  if (HasOptionalArgument(0))
    EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(0, &args));

  if (args) {
    std::string url_string;
    if (args->HasKey(keys::kUrlKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetString(keys::kUrlKey,
                                                  &url_string));
      url = ResolvePossiblyRelativeURL(url_string, GetExtension());
      if (!url.is_valid()) {
        error_ = ExtensionErrorUtils::FormatErrorMessage(
            keys::kInvalidUrlError, url_string);
        return false;
      }
    }
  }

  gfx::Rect empty_bounds;
  gfx::Rect bounds;
  bool maximized;
  WindowSizer::GetBrowserWindowBounds(std::wstring(), empty_bounds,
                                      GetCurrentBrowser(), &bounds,
                                      &maximized);

  Profile* window_profile = profile();
  Browser::Type window_type = Browser::TYPE_NORMAL;

  if (args) {
    int bounds_val;
    if (args->HasKey(keys::kLeftKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kLeftKey,
                                                   &bounds_val));
      bounds.set_x(bounds_val);
    }

    if (args->HasKey(keys::kTopKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kTopKey,
                                                   &bounds_val));
      bounds.set_y(bounds_val);
    }

    if (args->HasKey(keys::kWidthKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kWidthKey,
                                                   &bounds_val));
      bounds.set_width(bounds_val);
    }

    if (args->HasKey(keys::kHeightKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetInteger(keys::kHeightKey,
                                                   &bounds_val));
      bounds.set_height(bounds_val);
    }

    bool incognito = false;
    if (args->HasKey(keys::kIncognitoKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetBoolean(keys::kIncognitoKey,
                                                   &incognito));
      if (incognito)
        window_profile = window_profile->GetOffTheRecordProfile();
    }

    std::string type_str;
    if (args->HasKey(keys::kWindowTypeKey)) {
      EXTENSION_FUNCTION_VALIDATE(args->GetString(keys::kWindowTypeKey,
                                                  &type_str));
      if (type_str == keys::kWindowTypeValueNormal) {
        window_type = Browser::TYPE_NORMAL;
      } else if (type_str == keys::kWindowTypeValuePopup) {
        window_type = Browser::TYPE_POPUP;
      } else {
        EXTENSION_FUNCTION_VALIDATE(false);
      }
    }
  }

  Browser* new_window = new Browser(window_type, window_profile);
  new_window->CreateBrowserWindow();
  new_window->AddTabWithURL(url, GURL(), PageTransition::LINK, -1,
                            Browser::ADD_SELECTED, NULL, std::string());

  new_window->window()->SetBounds(bounds);
  new_window->window()->Show();

  if (new_window->profile()->IsOffTheRecord() && !include_incognito()) {
    result_.reset(Value::CreateNullValue());
  } else {
    result_.reset(ExtensionTabUtil::CreateWindowValue(new_window, false));
  }

  return true;
}
