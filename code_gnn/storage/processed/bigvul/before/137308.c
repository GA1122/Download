Textfield::Textfield()
    : model_(new TextfieldModel(this)),
      controller_(NULL),
      scheduled_text_edit_command_(ui::TextEditCommand::INVALID_COMMAND),
      read_only_(false),
      default_width_in_chars_(0),
      use_default_text_color_(true),
      use_default_background_color_(true),
      use_default_selection_text_color_(true),
      use_default_selection_background_color_(true),
      text_color_(SK_ColorBLACK),
      background_color_(SK_ColorWHITE),
      selection_text_color_(SK_ColorWHITE),
      selection_background_color_(SK_ColorBLUE),
      placeholder_text_draw_flags_(gfx::Canvas::DefaultCanvasTextAlignment()),
      invalid_(false),
      label_ax_id_(0),
      text_input_type_(ui::TEXT_INPUT_TYPE_TEXT),
      text_input_flags_(0),
      performing_user_action_(false),
      skip_input_method_cancel_composition_(false),
      drop_cursor_visible_(false),
      initiating_drag_(false),
      selection_controller_(this),
      drag_start_display_offset_(0),
      touch_handles_hidden_due_to_scroll_(false),
      use_focus_ring_(ui::MaterialDesignController::IsSecondaryUiMaterial()),
      weak_ptr_factory_(this) {
  set_context_menu_controller(this);
  set_drag_controller(this);
  cursor_view_.SetPaintToLayer(ui::LAYER_SOLID_COLOR);
  cursor_view_.layer()->SetColor(GetTextColor());
  cursor_view_.set_owned_by_client();
  AddChildView(&cursor_view_);
  GetRenderText()->SetFontList(GetDefaultFontList());
  UpdateBorder();
  SetFocusBehavior(FocusBehavior::ALWAYS);

#if !defined(OS_MACOSX)

  AddAccelerator(ui::Accelerator(ui::VKEY_X, ui::EF_CONTROL_DOWN));
  AddAccelerator(ui::Accelerator(ui::VKEY_C, ui::EF_CONTROL_DOWN));
  AddAccelerator(ui::Accelerator(ui::VKEY_V, ui::EF_CONTROL_DOWN));
#endif
}
