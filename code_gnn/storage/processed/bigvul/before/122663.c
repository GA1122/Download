bool Extension::HasMultipleUISurfaces() const {
  int num_surfaces = 0;

  if (page_action_info())
    ++num_surfaces;

  if (ActionInfo::GetBrowserActionInfo(this))
    ++num_surfaces;

  if (is_app())
    ++num_surfaces;

  return num_surfaces > 1;
}
