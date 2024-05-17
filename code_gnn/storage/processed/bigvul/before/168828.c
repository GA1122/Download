bool IsValidStateForWindowsCreateFunction(
    const windows::Create::Params::CreateData* create_data) {
  if (!create_data)
    return true;

  bool has_bound = create_data->left || create_data->top ||
                   create_data->width || create_data->height;
  bool is_panel = create_data->type == windows::CreateType::CREATE_TYPE_PANEL;

  switch (create_data->state) {
    case windows::WINDOW_STATE_MINIMIZED:
      return !(create_data->focused && *create_data->focused) && !has_bound &&
             !is_panel;
    case windows::WINDOW_STATE_MAXIMIZED:
    case windows::WINDOW_STATE_FULLSCREEN:
    case windows::WINDOW_STATE_LOCKED_FULLSCREEN:
      return !(create_data->focused && !*create_data->focused) && !has_bound &&
             !is_panel;
    case windows::WINDOW_STATE_NORMAL:
    case windows::WINDOW_STATE_DOCKED:
    case windows::WINDOW_STATE_NONE:
      return true;
  }
  NOTREACHED();
  return true;
}
