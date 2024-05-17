Instance::Instance(PP_Instance instance)
    : pp::InstancePrivate(instance),
      pp::Find_Private(this),
      pp::Printing_Dev(this),
      pp::Selection_Dev(this),
      pp::WidgetClient_Dev(this),
      pp::Zoom_Dev(this),
      cursor_(PP_CURSORTYPE_POINTER),
      timer_pending_(false),
      current_timer_id_(0),
      zoom_(1.0),
      device_scale_(1.0),
      printing_enabled_(true),
      hidpi_enabled_(false),
      full_(IsFullFrame()),
      zoom_mode_(full_ ? ZOOM_AUTO : ZOOM_SCALE),
      did_call_start_loading_(false),
      is_autoscroll_(false),
      scrollbar_thickness_(-1),
      scrollbar_reserved_thickness_(-1),
      current_tb_info_(NULL),
      current_tb_info_size_(0),
      paint_manager_(this, this, true),
      delayed_progress_timer_id_(0),
      first_paint_(true),
      painted_first_page_(false),
      show_page_indicator_(false),
      document_load_state_(LOAD_STATE_LOADING),
      preview_document_load_state_(LOAD_STATE_COMPLETE),
      told_browser_about_unsupported_feature_(false),
      print_preview_page_count_(0) {
  loader_factory_.Initialize(this);
  timer_factory_.Initialize(this);
  form_factory_.Initialize(this);
  callback_factory_.Initialize(this);
  engine_.reset(PDFEngine::Create(this));
  pp::Module::Get()->AddPluginInterface(kPPPPdfInterface, &ppp_private);
  AddPerInstanceObject(kPPPPdfInterface, this);

  RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_MOUSE);
  RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_WHEEL);
  RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_KEYBOARD);
  RequestFilteringInputEvents(PP_INPUTEVENT_CLASS_TOUCH);
}
