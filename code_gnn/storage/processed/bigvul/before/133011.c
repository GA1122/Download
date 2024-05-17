RenderWidgetHostViewAura::RenderWidgetHostViewAura(RenderWidgetHost* host)
    : host_(RenderWidgetHostImpl::From(host)),
      window_(new aura::Window(this)),
      in_shutdown_(false),
      in_bounds_changed_(false),
      is_fullscreen_(false),
      popup_parent_host_view_(NULL),
      popup_child_host_view_(NULL),
      is_loading_(false),
      text_input_type_(ui::TEXT_INPUT_TYPE_NONE),
      text_input_mode_(ui::TEXT_INPUT_MODE_DEFAULT),
      can_compose_inline_(true),
      has_composition_text_(false),
      accept_return_character_(false),
      last_output_surface_id_(0),
      pending_delegated_ack_count_(0),
      skipped_frames_(false),
      last_swapped_surface_scale_factor_(1.f),
      paint_canvas_(NULL),
      synthetic_move_sent_(false),
      accelerated_compositing_state_changed_(false),
      can_lock_compositor_(YES),
      cursor_visibility_state_in_renderer_(UNKNOWN),
      paint_observer_(NULL),
      touch_editing_client_(NULL),
      delegated_frame_evictor_(new DelegatedFrameEvictor(this)),
      weak_ptr_factory_(this) {
  host_->SetView(this);
  window_observer_.reset(new WindowObserver(this));
  aura::client::SetTooltipText(window_, &tooltip_);
  aura::client::SetActivationDelegate(window_, this);
  aura::client::SetActivationChangeObserver(window_, this);
  aura::client::SetFocusChangeObserver(window_, this);
  gfx::Screen::GetScreenFor(window_)->AddObserver(this);
  software_frame_manager_.reset(new SoftwareFrameManager(
      weak_ptr_factory_.GetWeakPtr()));
#if defined(OS_WIN)
  plugin_parent_window_ = NULL;
#endif
  ImageTransportFactory::GetInstance()->AddObserver(this);
}
