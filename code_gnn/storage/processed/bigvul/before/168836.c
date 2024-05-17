ExtensionFunction::ResponseAction WindowsUpdateFunction::Run() {
  std::unique_ptr<windows::Update::Params> params(
      windows::Update::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params);

  Browser* browser = nullptr;
  std::string error;
  if (!windows_util::GetBrowserFromWindowID(
          this, params->window_id, WindowController::GetAllWindowFilter(),
          &browser, &error)) {
    return RespondNow(Error(error));
  }

  ReportRequestedWindowState(params->update_info.state);

  if (params->update_info.state == windows::WINDOW_STATE_LOCKED_FULLSCREEN &&
      !ExtensionHasLockedFullscreenPermission(extension())) {
    return RespondNow(
        Error(keys::kMissingLockWindowFullscreenPrivatePermission));
  }

#if defined(OS_CHROMEOS)
  const bool is_window_trusted_pinned =
      ash::IsWindowTrustedPinned(browser->window());
  if (is_window_trusted_pinned &&
      !ExtensionHasLockedFullscreenPermission(extension())) {
    return RespondNow(
        Error(keys::kMissingLockWindowFullscreenPrivatePermission));
  }
  if (is_window_trusted_pinned &&
      params->update_info.state != windows::WINDOW_STATE_LOCKED_FULLSCREEN &&
      params->update_info.state != windows::WINDOW_STATE_NONE) {
    SetLockedFullscreenState(browser, false);
  } else if (!is_window_trusted_pinned &&
             params->update_info.state ==
                 windows::WINDOW_STATE_LOCKED_FULLSCREEN) {
    SetLockedFullscreenState(browser, true);
  }
#endif

  ui::WindowShowState show_state =
      ConvertToWindowShowState(params->update_info.state);

  if (show_state != ui::SHOW_STATE_FULLSCREEN &&
      show_state != ui::SHOW_STATE_DEFAULT) {
    browser->extension_window_controller()->SetFullscreenMode(
        false, extension()->url());
  }

  switch (show_state) {
    case ui::SHOW_STATE_MINIMIZED:
      browser->window()->Minimize();
      break;
    case ui::SHOW_STATE_MAXIMIZED:
      browser->window()->Maximize();
      break;
    case ui::SHOW_STATE_FULLSCREEN:
      if (browser->window()->IsMinimized() ||
          browser->window()->IsMaximized()) {
        browser->window()->Restore();
      }
      browser->extension_window_controller()->SetFullscreenMode(
          true, extension()->url());
      break;
    case ui::SHOW_STATE_NORMAL:
      browser->window()->Restore();
      break;
    default:
      break;
  }

  gfx::Rect bounds;
  if (browser->window()->IsMinimized())
    bounds = browser->window()->GetRestoredBounds();
  else
    bounds = browser->window()->GetBounds();
  bool set_bounds = false;

  if (params->update_info.left) {
    bounds.set_x(*params->update_info.left);
    set_bounds = true;
  }

  if (params->update_info.top) {
    bounds.set_y(*params->update_info.top);
    set_bounds = true;
  }

  if (params->update_info.width) {
    bounds.set_width(*params->update_info.width);
    set_bounds = true;
  }

  if (params->update_info.height) {
    bounds.set_height(*params->update_info.height);
    set_bounds = true;
  }

  if (set_bounds) {
    if (show_state == ui::SHOW_STATE_MINIMIZED ||
        show_state == ui::SHOW_STATE_MAXIMIZED ||
        show_state == ui::SHOW_STATE_FULLSCREEN) {
      return RespondNow(Error(keys::kInvalidWindowStateError));
    }
    browser->window()->SetBounds(bounds);
  }

  if (params->update_info.focused) {
    if (*params->update_info.focused) {
      if (show_state == ui::SHOW_STATE_MINIMIZED)
        return RespondNow(Error(keys::kInvalidWindowStateError));
      browser->window()->Activate();
    } else {
      if (show_state == ui::SHOW_STATE_MAXIMIZED ||
          show_state == ui::SHOW_STATE_FULLSCREEN) {
        return RespondNow(Error(keys::kInvalidWindowStateError));
      }
      browser->window()->Deactivate();
    }
  }

  if (params->update_info.draw_attention)
    browser->window()->FlashFrame(*params->update_info.draw_attention);

  return RespondNow(OneArgument(ExtensionTabUtil::CreateWindowValueForExtension(
      *browser, extension(), ExtensionTabUtil::kDontPopulateTabs)));
}
