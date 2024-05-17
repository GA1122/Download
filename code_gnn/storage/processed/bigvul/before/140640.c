OutOfProcessInstance::OutOfProcessInstance(PP_Instance instance)
    : pp::Instance(instance),
      pp::Find_Private(this),
      pp::Printing_Dev(this),
      cursor_(PP_CURSORTYPE_POINTER),
      zoom_(1.0),
      device_scale_(1.0),
      full_(false),
      paint_manager_(this, this, true),
      first_paint_(true),
      document_load_state_(LOAD_STATE_LOADING),
      preview_document_load_state_(LOAD_STATE_COMPLETE),
      uma_(this),
      told_browser_about_unsupported_feature_(false),
      print_preview_page_count_(0),
      last_progress_sent_(0),
      recently_sent_find_update_(false),
      received_viewport_message_(false),
      did_call_start_loading_(false),
      stop_scrolling_(false),
      background_color_(kBackgroundColor),
      top_toolbar_height_(0) {
  loader_factory_.Initialize(this);
  timer_factory_.Initialize(this);
  form_factory_.Initialize(this);
  print_callback_factory_.Initialize(this);
  engine_.reset(PDFEngine::Create(this));
  pp::Module::Get()->AddPluginInterface(kPPPPdfInterface, &ppp_private);
  AddPerInstanceObject(kPPPPdfInterface, this);

  RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_MOUSE);
  RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_KEYBOARD);
  RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_TOUCH);
}
