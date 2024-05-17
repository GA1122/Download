std::unique_ptr<WebUIImpl> WebContentsImpl::CreateWebUI(
    const GURL& url,
    const std::string& frame_name) {
  std::unique_ptr<WebUIImpl> web_ui =
      base::MakeUnique<WebUIImpl>(this, frame_name);
  WebUIController* controller =
      WebUIControllerFactoryRegistry::GetInstance()
          ->CreateWebUIControllerForURL(web_ui.get(), url);
  if (controller) {
    web_ui->AddMessageHandler(base::MakeUnique<GenericHandler>());
    web_ui->SetController(controller);
    return web_ui;
  }

  return nullptr;
}
