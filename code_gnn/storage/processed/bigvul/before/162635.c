HeadlessDevToolsManagerDelegate::SetWindowBounds(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  HeadlessWebContentsImpl* web_contents;
  const base::Value* window_id_value = params->FindKey("windowId");
  if (!window_id_value || !window_id_value->is_int())
    return CreateInvalidParamResponse(command_id, "windowId");
  web_contents = browser_->GetWebContentsForWindowId(window_id_value->GetInt());

  if (!web_contents) {
    return CreateErrorResponse(command_id, kErrorServerError,
                               "Browser window not found");
  }

  const base::Value* bounds_value = params->FindKey("bounds");
  if (!bounds_value || !bounds_value->is_dict())
    return CreateInvalidParamResponse(command_id, "bounds");

  std::string window_state;
  if (const base::Value* window_state_value =
          bounds_value->FindKey("windowState")) {
    window_state = window_state_value->GetString();
    if (window_state != "normal" && window_state != "minimized" &&
        window_state != "maximized" && window_state != "fullscreen") {
      return CreateInvalidParamResponse(command_id, "windowState");
    }
  } else {
    window_state = "normal";
  }

  bool set_bounds = false;
  gfx::Rect bounds = web_contents->web_contents()->GetContainerBounds();
  if (const base::Value* left_value = bounds_value->FindKey("left")) {
    bounds.set_x(left_value->GetInt());
    set_bounds = true;
  }

  if (const base::Value* top_value = bounds_value->FindKey("top")) {
    bounds.set_y(top_value->GetInt());
    set_bounds = true;
  }

  if (const base::Value* width_value = bounds_value->FindKey("width")) {
    int width = width_value->GetInt();
    if (width < 0)
      return CreateInvalidParamResponse(command_id, "width");
    bounds.set_width(width);
    set_bounds = true;
  }

  if (const base::Value* height_value = bounds_value->FindKey("height")) {
    int height = height_value->GetInt();
    if (height < 0)
      return CreateInvalidParamResponse(command_id, "height");
    bounds.set_height(height);
    set_bounds = true;
  }

  if (set_bounds && window_state != "normal") {
    return CreateErrorResponse(
        command_id, kErrorServerError,
        "The 'minimized', 'maximized' and 'fullscreen' states cannot be "
        "combined with 'left', 'top', 'width' or 'height'");
  }

  if (set_bounds && web_contents->window_state() != "normal") {
    return CreateErrorResponse(
        command_id, kErrorServerError,
        "To resize minimized/maximized/fullscreen window, restore it to normal "
        "state first.");
  }

  web_contents->set_window_state(window_state);
  web_contents->SetBounds(bounds);
  return CreateSuccessResponse(command_id, nullptr);
}
