void AppLauncherHandler::PromptToEnableApp(const std::string& extension_id) {
  if (!extension_id_prompting_.empty())
    return;   

  extension_id_prompting_ = extension_id;
  extension_enable_flow_.reset(new ExtensionEnableFlow(
      Profile::FromWebUI(web_ui()), extension_id, this));
  extension_enable_flow_->StartForWebContents(web_ui()->GetWebContents());
}
