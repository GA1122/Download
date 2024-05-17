void DesktopWindowTreeHostX11::InitX11Window(
    const Widget::InitParams& params) {
  unsigned long attribute_mask = CWBackPixel | CWBitGravity;
  XSetWindowAttributes swa;
  memset(&swa, 0, sizeof(swa));
  swa.background_pixmap = x11::None;
  swa.bit_gravity = NorthWestGravity;

  int background_color;

  const views::LinuxUI* linux_ui = views::LinuxUI::instance();
  if (linux_ui && content_window()) {
    ui::NativeTheme::ColorId target_color;
    switch (params.type) {
      case Widget::InitParams::TYPE_BUBBLE:
        target_color = ui::NativeTheme::kColorId_BubbleBackground;
        break;
      case Widget::InitParams::TYPE_TOOLTIP:
        target_color = ui::NativeTheme::kColorId_TooltipBackground;
        break;
      default:
        target_color = ui::NativeTheme::kColorId_WindowBackground;
        break;
    }

    ui::NativeTheme* theme = linux_ui->GetNativeTheme(content_window());
    background_color = theme->GetSystemColor(target_color);
  } else {
    background_color = WhitePixel(xdisplay_, DefaultScreen(xdisplay_));
  }
  swa.background_pixel = background_color;

  XAtom window_type;
  switch (params.type) {
    case Widget::InitParams::TYPE_MENU:
      swa.override_redirect = x11::True;
      window_type = gfx::GetAtom("_NET_WM_WINDOW_TYPE_MENU");
      break;
    case Widget::InitParams::TYPE_TOOLTIP:
      swa.override_redirect = x11::True;
      window_type = gfx::GetAtom("_NET_WM_WINDOW_TYPE_TOOLTIP");
      break;
    case Widget::InitParams::TYPE_POPUP:
      swa.override_redirect = x11::True;
      window_type = gfx::GetAtom("_NET_WM_WINDOW_TYPE_NOTIFICATION");
      break;
    case Widget::InitParams::TYPE_DRAG:
      swa.override_redirect = x11::True;
      window_type = gfx::GetAtom("_NET_WM_WINDOW_TYPE_DND");
      break;
    default:
      window_type = gfx::GetAtom("_NET_WM_WINDOW_TYPE_NORMAL");
      break;
  }
  if (!activatable_)
    swa.override_redirect = x11::True;

  override_redirect_ = swa.override_redirect == x11::True;
  if (override_redirect_)
    attribute_mask |= CWOverrideRedirect;

  bool enable_transparent_visuals;
  switch (params.opacity) {
    case Widget::InitParams::OPAQUE_WINDOW:
      enable_transparent_visuals = false;
      break;
    case Widget::InitParams::TRANSLUCENT_WINDOW:
      enable_transparent_visuals = true;
      break;
    case Widget::InitParams::INFER_OPACITY:
    default:
      enable_transparent_visuals = params.type == Widget::InitParams::TYPE_DRAG;
  }

  Visual* visual = CopyFromParent;
  int depth = CopyFromParent;
  Colormap colormap = CopyFromParent;
  ui::XVisualManager::GetInstance()->ChooseVisualForWindow(
      enable_transparent_visuals, &visual, &depth, &colormap,
      &use_argb_visual_);

  if (colormap != CopyFromParent) {
    attribute_mask |= CWColormap;
    swa.colormap = colormap;
  }

  attribute_mask |= CWBorderPixel;
  swa.border_pixel = 0;

  bounds_in_pixels_ = ToPixelRect(params.bounds);
  bounds_in_pixels_.set_size(AdjustSize(bounds_in_pixels_.size()));
  xwindow_ = XCreateWindow(xdisplay_, x_root_window_, bounds_in_pixels_.x(),
                           bounds_in_pixels_.y(), bounds_in_pixels_.width(),
                           bounds_in_pixels_.height(),
                           0,   
                           depth, InputOutput, visual, attribute_mask, &swa);
  if (ui::PlatformEventSource::GetInstance())
    ui::PlatformEventSource::GetInstance()->AddPlatformEventDispatcher(this);
  open_windows().push_front(xwindow_);


  long event_mask = ButtonPressMask | ButtonReleaseMask | FocusChangeMask |
                    KeyPressMask | KeyReleaseMask |
                    EnterWindowMask | LeaveWindowMask |
                    ExposureMask | VisibilityChangeMask |
                    StructureNotifyMask | PropertyChangeMask |
                    PointerMotionMask;
  xwindow_events_ =
      std::make_unique<ui::XScopedEventSelector>(xwindow_, event_mask);
  XFlush(xdisplay_);

  if (ui::IsXInput2Available())
    ui::TouchFactory::GetInstance()->SetupXI2ForXWindow(xwindow_);

  XAtom protocols[2];
  protocols[0] = gfx::GetAtom("WM_DELETE_WINDOW");
  protocols[1] = gfx::GetAtom("_NET_WM_PING");
  XSetWMProtocols(xdisplay_, xwindow_, protocols, 2);

  XSetWMProperties(xdisplay_, xwindow_, nullptr, nullptr, nullptr, 0, nullptr,
                   nullptr, nullptr);

  static_assert(sizeof(long) >= sizeof(pid_t),
                "pid_t should not be larger than long");
  long pid = getpid();
  XChangeProperty(xdisplay_, xwindow_, gfx::GetAtom("_NET_WM_PID"), XA_CARDINAL,
                  32, PropModeReplace, reinterpret_cast<unsigned char*>(&pid),
                  1);

  XChangeProperty(xdisplay_, xwindow_, gfx::GetAtom("_NET_WM_WINDOW_TYPE"),
                  XA_ATOM, 32, PropModeReplace,
                  reinterpret_cast<unsigned char*>(&window_type), 1);


  if ((params.type == Widget::InitParams::TYPE_POPUP ||
       params.type == Widget::InitParams::TYPE_BUBBLE) &&
      !params.force_show_in_taskbar) {
    window_properties_.insert(gfx::GetAtom("_NET_WM_STATE_SKIP_TASKBAR"));
  }

  is_always_on_top_ = params.keep_on_top;
  if (is_always_on_top_)
    window_properties_.insert(gfx::GetAtom("_NET_WM_STATE_ABOVE"));

  workspace_ = base::nullopt;
  if (params.visible_on_all_workspaces) {
    window_properties_.insert(gfx::GetAtom("_NET_WM_STATE_STICKY"));
    ui::SetIntProperty(xwindow_, "_NET_WM_DESKTOP", "CARDINAL", kAllDesktops);
  } else if (!params.workspace.empty()) {
    int workspace;
    if (base::StringToInt(params.workspace, &workspace))
      ui::SetIntProperty(xwindow_, "_NET_WM_DESKTOP", "CARDINAL", workspace);
  }

  if (!params.wm_class_name.empty() || !params.wm_class_class.empty()) {
    ui::SetWindowClassHint(
        xdisplay_, xwindow_, params.wm_class_name, params.wm_class_class);
  }

  const char* wm_role_name = nullptr;
  if (!params.wm_role_name.empty()) {
    wm_role_name = params.wm_role_name.c_str();
  } else {
    switch (params.type) {
      case Widget::InitParams::TYPE_POPUP:
        wm_role_name = kX11WindowRolePopup;
        break;
      case Widget::InitParams::TYPE_BUBBLE:
        wm_role_name = kX11WindowRoleBubble;
        break;
      default:
        break;
    }
  }
  if (wm_role_name)
    ui::SetWindowRole(xdisplay_, xwindow_, std::string(wm_role_name));

  if (params.remove_standard_frame) {
    ui::SetHideTitlebarWhenMaximizedProperty(xwindow_,
                                             ui::HIDE_TITLEBAR_WHEN_MAXIMIZED);
  }

  if (views::LinuxUI::instance() &&
      views::LinuxUI::instance()->PreferDarkTheme()) {
    const unsigned char kDarkGtkThemeVariant[] = "dark";
    XChangeProperty(xdisplay_, xwindow_, gfx::GetAtom("_GTK_THEME_VARIANT"),
                    gfx::GetAtom("UTF8_STRING"), 8, PropModeReplace,
                    kDarkGtkThemeVariant, base::size(kDarkGtkThemeVariant) - 1);
  }

  ui::SetIntProperty(xwindow_, "_NET_WM_BYPASS_COMPOSITOR", "CARDINAL", 2);

  if (params.parent && params.parent->GetHost()) {
    XID parent_xid =
        params.parent->GetHost()->GetAcceleratedWidget();
    window_parent_ = GetHostForXID(parent_xid);
    DCHECK(window_parent_);
    window_parent_->window_children_.insert(this);
  }

  gfx::ImageSkia* window_icon =
      ViewsDelegate::GetInstance()
          ? ViewsDelegate::GetInstance()->GetDefaultWindowIcon()
          : nullptr;
  if (window_icon) {
    SetWindowIcons(gfx::ImageSkia(), *window_icon);
  }
  CreateCompositor(viz::FrameSinkId(),
                   params.type == Widget::InitParams::TYPE_TOOLTIP);
  OnAcceleratedWidgetAvailable();
}
