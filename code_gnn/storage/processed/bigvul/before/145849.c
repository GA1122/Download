HeadlessDevToolsManagerDelegate::SetWindowBounds(
    content::DevToolsAgentHost* agent_host,
    int session_id,
    int command_id,
    const base::DictionaryValue* params) {
  int window_id;
  if (!params->GetInteger("windowId", &window_id))
    return CreateInvalidParamResponse(command_id, "windowId");
  HeadlessWebContentsImpl* web_contents =
      browser_->GetWebContentsForWindowId(window_id);
  if (!web_contents) {
    return CreateErrorResponse(command_id, kErrorServerError,
                               "Browser window not found");
  }

  const base::Value* value = nullptr;
  const base::DictionaryValue* bounds_dict = nullptr;
  if (!params->Get("bounds", &value) || !value->GetAsDictionary(&bounds_dict))
    return CreateInvalidParamResponse(command_id, "bounds");

  std::string window_state;
  if (!bounds_dict->GetString("windowState", &window_state)) {
    window_state = "normal";
  } else if (window_state != "normal" && window_state != "minimized" &&
             window_state != "maximized" && window_state != "fullscreen") {
    return CreateInvalidParamResponse(command_id, "windowState");
  }

  bool set_bounds = false;
  gfx::Rect bounds = web_contents->web_contents()->GetContainerBounds();
  int left, top, width, height;
  if (bounds_dict->GetInteger("left", &left)) {
    bounds.set_x(left);
    set_bounds = true;
  }
  if (bounds_dict->GetInteger("top", &top)) {
    bounds.set_y(top);
    set_bounds = true;
  }
  if (bounds_dict->GetInteger("width", &width)) {
    if (width < 0)
      return CreateInvalidParamResponse(command_id, "width");
    bounds.set_width(width);
    set_bounds = true;
  }
  if (bounds_dict->GetInteger("height", &height)) {
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
