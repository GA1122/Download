void BlockedPlugin::OnMenuItemSelected(
    const webkit_glue::CustomContextMenuContext&  ,
    unsigned id) {
  if (id == kMenuActionLoad) {
    LoadPlugin();
  } else if (id == kMenuActionRemove) {
    HidePlugin();
  }
}
