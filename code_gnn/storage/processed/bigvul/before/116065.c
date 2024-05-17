void ExtensionSettingsHandler::HandleInspectMessage(const ListValue* args) {
  std::string render_process_id_str;
  std::string render_view_id_str;
  int render_process_id;
  int render_view_id;
  CHECK_EQ(2U, args->GetSize());
  CHECK(args->GetString(0, &render_process_id_str));
  CHECK(args->GetString(1, &render_view_id_str));
  CHECK(base::StringToInt(render_process_id_str, &render_process_id));
  CHECK(base::StringToInt(render_view_id_str, &render_view_id));
  RenderViewHost* host = RenderViewHost::FromID(render_process_id,
                                                render_view_id);
  if (!host) {
    return;
  }

  DevToolsWindow::OpenDevToolsWindow(host);
}
