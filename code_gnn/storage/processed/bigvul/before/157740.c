std::unique_ptr<WebUIImpl> WebContentsImpl::CreateWebUI(const GURL& url) {
  std::unique_ptr<WebUIImpl> web_ui = std::make_unique<WebUIImpl>(this);
  std::unique_ptr<WebUIController> controller(
      WebUIControllerFactoryRegistry::GetInstance()
          ->CreateWebUIControllerForURL(web_ui.get(), url));
  if (controller) {
    web_ui->AddMessageHandler(std::make_unique<GenericHandler>());
    web_ui->SetController(std::move(controller));
    return web_ui;
  }

  return nullptr;
}
