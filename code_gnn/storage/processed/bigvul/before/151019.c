void DevToolsUIBindings::Reload() {
  reloading_ = true;
  web_contents_->GetController().Reload(content::ReloadType::NORMAL, false);
}
