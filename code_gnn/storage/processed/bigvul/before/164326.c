bool IsValidStateForWindowsCreateFunction(
    const windows::Create::Params::CreateData* create_data) {
  if (!create_data)
    return true;

  bool has_bound = create_data->left || create_data->top ||
                   create_data->width || create_data->height;

  switch (create_data->state) {
    case windows::WINDOW_STATE_MINIMIZED:
      return !(create_data->focused && *create_data->focused) && !has_bound;
    case windows::WINDOW_STATE_MAXIMIZED:
    case windows::WINDOW_STATE_FULLSCREEN:
    case windows::WINDOW_STATE_LOCKED_FULLSCREEN:
      return !(create_data->focused && !*create_data->focused) && !has_bound;
    case windows::WINDOW_STATE_NORMAL:
    case windows::WINDOW_STATE_DOCKED:
    case windows::WINDOW_STATE_NONE:
      return true;
  }
  NOTREACHED();
  return true;
}
