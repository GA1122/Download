pp::Var Instance::CallScriptableMethod(const pp::Var& method,
                                       const std::vector<pp::Var>& args,
                                       pp::Var* exception) {
  std::string method_str = method.AsString();
  if (method_str == kJSGrayscale) {
    if (args.size() == 1 && args[0].is_bool()) {
      engine_->SetGrayscale(args[0].AsBool());
      paint_manager_.InvalidateRect(pp::Rect(pp::Point(), plugin_size_));
#ifdef ENABLE_THUMBNAILS
      if (thumbnails_.visible())
        thumbnails_.Show(true, true);
#endif
      return pp::Var(true);
    }
    return pp::Var(false);
  }
  if (method_str == kJSOnLoad) {
    if (args.size() == 1 && args[0].is_string()) {
      on_load_callback_ = args[0];
      return pp::Var(true);
    }
    return pp::Var(false);
  }
  if (method_str == kJSOnScroll) {
    if (args.size() == 1 && args[0].is_string()) {
      on_scroll_callback_ = args[0];
      return pp::Var(true);
    }
    return pp::Var(false);
  }
  if (method_str == kJSOnPluginSizeChanged) {
    if (args.size() == 1 && args[0].is_string()) {
      on_plugin_size_changed_callback_ = args[0];
      return pp::Var(true);
    }
    return pp::Var(false);
  }
  if (method_str == kJSReload) {
    document_load_state_ = LOAD_STATE_LOADING;
    if (!full_)
      LoadUrl(url_);
    preview_engine_.reset();
    print_preview_page_count_ = 0;
    engine_.reset(PDFEngine::Create(this));
    engine_->New(url_.c_str());
#ifdef ENABLE_THUMBNAILS
    thumbnails_.ResetEngine(engine_.get());
#endif
    return pp::Var();
  }
  if (method_str == kJSResetPrintPreviewUrl) {
    if (args.size() == 1 && args[0].is_string()) {
      url_ = args[0].AsString();
      preview_pages_info_ = std::queue<PreviewPageInfo>();
      preview_document_load_state_ = LOAD_STATE_COMPLETE;
    }
    return pp::Var();
  }
  if (method_str == kJSZoomFitToHeight) {
    SetZoom(ZOOM_FIT_TO_PAGE, 0);
    return pp::Var();
  }
  if (method_str == kJSZoomFitToWidth) {
    SetZoom(ZOOM_FIT_TO_WIDTH, 0);
    return pp::Var();
  }
  if (method_str == kJSZoomIn) {
    SetZoom(ZOOM_SCALE, CalculateZoom(kZoomInButtonId));
    return pp::Var();
  }
  if (method_str == kJSZoomOut) {
    SetZoom(ZOOM_SCALE, CalculateZoom(kZoomOutButtonId));
    return pp::Var();
  }
  if (method_str == kJSSetZoomLevel) {
    if (args.size() == 1 && args[0].is_number())
      SetZoom(ZOOM_SCALE, args[0].AsDouble());
    return pp::Var();
  }
  if (method_str == kJSGetZoomLevel) {
    return pp::Var(zoom_);
  }
  if (method_str == kJSGetHeight) {
    return pp::Var(plugin_size_.height());
  }
  if (method_str == kJSGetWidth) {
    return pp::Var(plugin_size_.width());
  }
  if (method_str == kJSGetHorizontalScrollbarThickness) {
    return pp::Var(
          h_scrollbar_.get() ? GetScrollbarReservedThickness() : 0);
  }
  if (method_str == kJSGetVerticalScrollbarThickness) {
    return pp::Var(
          v_scrollbar_.get() ? GetScrollbarReservedThickness() : 0);
  }
  if (method_str == kJSDocumentLoadComplete) {
    return pp::Var((document_load_state_ != LOAD_STATE_LOADING));
  }
  if (method_str == kJSPageYOffset) {
    return pp::Var(static_cast<int32_t>(
        v_scrollbar_.get() ? v_scrollbar_->GetValue() : 0));
  }
  if (method_str == kJSSetPageYOffset) {
    if (args.size() == 1 && args[0].is_number() && v_scrollbar_.get())
      ScrollToY(GetScaled(args[0].AsInt()));
    return pp::Var();
  }
  if (method_str == kJSPageXOffset) {
    return pp::Var(static_cast<int32_t>(
        h_scrollbar_.get() ? h_scrollbar_->GetValue() : 0));
  }
  if (method_str == kJSSetPageXOffset) {
    if (args.size() == 1 && args[0].is_number() && h_scrollbar_.get())
      ScrollToX(GetScaled(args[0].AsInt()));
    return pp::Var();
  }
  if (method_str == kJSRemovePrintButton) {
    CreateToolbar(kPrintPreviewToolbarButtons,
                  arraysize(kPrintPreviewToolbarButtons));
    UpdateToolbarPosition(false);
    Invalidate(pp::Rect(plugin_size_));
    return pp::Var();
  }
  if (method_str == kJSGoToPage) {
    if (args.size() == 1 && args[0].is_string()) {
      ScrollToPage(atoi(args[0].AsString().c_str()));
    }
    return pp::Var();
  }
  if (method_str == kJSAccessibility) {
    if (args.size() == 0) {
      base::DictionaryValue node;
      node.SetInteger(kAccessibleNumberOfPages, engine_->GetNumberOfPages());
      node.SetBoolean(kAccessibleLoaded,
                      document_load_state_ != LOAD_STATE_LOADING);
      bool has_permissions =
          engine_->HasPermission(PDFEngine::PERMISSION_COPY) ||
          engine_->HasPermission(PDFEngine::PERMISSION_COPY_ACCESSIBLE);
      node.SetBoolean(kAccessibleCopyable, has_permissions);
      std::string json;
      base::JSONWriter::Write(&node, &json);
      return pp::Var(json);
    } else if (args[0].is_number()) {
      return pp::Var(engine_->GetPageAsJSON(args[0].AsInt()));
    }
  }
  if (method_str == kJSPrintPreviewPageCount) {
    if (args.size() == 1 && args[0].is_number())
      SetPrintPreviewMode(args[0].AsInt());
    return pp::Var();
  }
  if (method_str == kJSLoadPreviewPage) {
    if (args.size() == 2 && args[0].is_string() && args[1].is_number())
      ProcessPreviewPageInfo(args[0].AsString(), args[1].AsInt());
    return pp::Var();
  }
  if (method_str == kJSGetPageLocationNormalized) {
    const size_t kMaxLength = 30;
    char location_info[kMaxLength];
    int page_idx = engine_->GetMostVisiblePage();
    if (page_idx < 0)
      return pp::Var(std::string());
    pp::Rect rect = engine_->GetPageContentsRect(page_idx);
    int v_scrollbar_reserved_thickness =
        v_scrollbar_.get() ? GetScaled(GetScrollbarReservedThickness()) : 0;

    rect.set_x(rect.x() + ((plugin_size_.width() -
        v_scrollbar_reserved_thickness - available_area_.width()) / 2));
    base::snprintf(location_info,
                   kMaxLength,
                   "%0.4f;%0.4f;%0.4f;%0.4f;",
                   rect.x() / static_cast<float>(plugin_size_.width()),
                   rect.y() / static_cast<float>(plugin_size_.height()),
                   rect.width() / static_cast<float>(plugin_size_.width()),
                   rect.height()/ static_cast<float>(plugin_size_.height()));
    return pp::Var(std::string(location_info));
  }
  if (method_str == kJSSetPageNumbers) {
    if (args.size() != 1 || !args[0].is_string())
      return pp::Var();
    const int num_pages_signed = engine_->GetNumberOfPages();
    if (num_pages_signed <= 0)
      return pp::Var();
    scoped_ptr<base::ListValue> page_ranges(static_cast<base::ListValue*>(
        base::JSONReader::Read(args[0].AsString(), false)));
    const size_t num_pages = static_cast<size_t>(num_pages_signed);
    if (!page_ranges.get() || page_ranges->GetSize() != num_pages)
      return pp::Var();

    std::vector<int> print_preview_page_numbers;
    for (size_t index = 0; index < num_pages; ++index) {
      int page_number = 0;   
      if (!page_ranges->GetInteger(index, &page_number) || page_number < 1)
        return pp::Var();
      print_preview_page_numbers.push_back(page_number);
    }
    print_preview_page_numbers_ = print_preview_page_numbers;
    page_indicator_.set_current_page(GetPageNumberToDisplay());
    return pp::Var();
  }
  if (method_str == kJSSendKeyEvent) {
    if (args.size() == 1 && args[0].is_number()) {
      pp::KeyboardInputEvent event(
          this,                         
          PP_INPUTEVENT_TYPE_KEYDOWN,   
          0,                            
          0,                            
          args[0].AsInt(),              
          pp::Var());                   
      HandleInputEvent(event);
    }
  }
  return pp::Var();
}
