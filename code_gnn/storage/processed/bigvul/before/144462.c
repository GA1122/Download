WebUI* WebContentsImpl::CreateWebUI(const GURL& url,
                                    const std::string& frame_name) {
  WebUIImpl* web_ui = new WebUIImpl(this, frame_name);
  WebUIController* controller = WebUIControllerFactoryRegistry::GetInstance()->
      CreateWebUIControllerForURL(web_ui, url);
  if (controller) {
    web_ui->AddMessageHandler(new GenericHandler());
    web_ui->SetController(controller);
    return web_ui;
  }

  delete web_ui;
  return NULL;
}
