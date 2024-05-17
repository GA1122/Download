bool CaptureVisibleTabFunction::RunImpl() {
  Browser* browser;
  int window_id = -1;

  if (HasOptionalArgument(0)) {
    EXTENSION_FUNCTION_VALIDATE(args_->GetInteger(0, &window_id));
    browser = GetBrowserInProfileWithId(profile(), window_id,
                                        include_incognito(), &error_);
  } else {
    browser = GetCurrentBrowser();
  }

  if (!browser) {
    error_ = keys::kNoCurrentWindowError;
    return false;
  }

  image_format_ = FORMAT_JPEG;   
  image_quality_ = kDefaultQuality;   

  if (HasOptionalArgument(1)) {
    DictionaryValue* options;
    EXTENSION_FUNCTION_VALIDATE(args_->GetDictionary(1, &options));

    if (options->HasKey(keys::kFormatKey)) {
      std::string format;
      EXTENSION_FUNCTION_VALIDATE(
          options->GetString(keys::kFormatKey, &format));

      if (format == keys::kFormatValueJpeg) {
        image_format_ = FORMAT_JPEG;
      } else if (format == keys::kFormatValuePng) {
        image_format_ = FORMAT_PNG;
      } else {
        EXTENSION_FUNCTION_VALIDATE(0);
      }
    }

    if (options->HasKey(keys::kQualityKey)) {
      EXTENSION_FUNCTION_VALIDATE(
          options->GetInteger(keys::kQualityKey, &image_quality_));
    }
  }

  TabContents* tab_contents = browser->GetSelectedTabContents();
  if (!tab_contents) {
    error_ = keys::kInternalVisibleTabCaptureError;
    return false;
  }
  RenderViewHost* render_view_host = tab_contents->render_view_host();

  BackingStore* backing_store = render_view_host->GetBackingStore(false);
  if (backing_store && CaptureSnapshotFromBackingStore(backing_store))
    return true;

  render_view_host->CaptureSnapshot();
  registrar_.Add(this,
                 NotificationType::TAB_SNAPSHOT_TAKEN,
                 NotificationService::AllSources());
  AddRef();   

  return true;
}
