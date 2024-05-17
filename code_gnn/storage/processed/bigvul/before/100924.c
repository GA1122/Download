bool TaskManagerHandler::is_alive() {
  return web_ui_->tab_contents()->GetRenderViewHost() != NULL;
}
