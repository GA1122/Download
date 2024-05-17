 void WebRunnerBrowserMainParts::PreMainMessageLoopRun() {
   DCHECK(!screen_);
 
   auto platform_screen = ui::OzonePlatform::GetInstance()->CreateScreen();
   if (platform_screen) {
     screen_ = std::make_unique<aura::ScreenOzone>(std::move(platform_screen));
   } else {
     screen_ = std::make_unique<WebRunnerScreen>();
   }
 
   display::Screen::SetScreenInstance(screen_.get());
 
   DCHECK(!browser_context_);
    browser_context_ =
        std::make_unique<WebRunnerBrowserContext>(GetWebContextDataDir());
  
  fidl::InterfaceRequest<chromium::web::Context> context_request(
      std::move(context_channel_));
//   context_service_ = std::make_unique<ContextImpl>(browser_context_.get());
  
  context_impl_ = std::make_unique<ContextImpl>(browser_context_.get());
    context_binding_ = std::make_unique<fidl::Binding<chromium::web::Context>>(
      context_impl_.get(), std::move(context_request));
//       context_service_.get(), fidl::InterfaceRequest<chromium::web::Context>(
//                                   std::move(context_channel_)));
  
  context_binding_->set_error_handler(
      [this]() { std::move(quit_closure_).Run(); });
//    
//   context_binding_->set_error_handler([this]() {
//     DLOG(WARNING) << "Client connection to Context service dropped.";
//     context_service_.reset();
//     std::move(quit_closure_).Run();
//   });
  }