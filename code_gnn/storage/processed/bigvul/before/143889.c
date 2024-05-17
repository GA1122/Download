void FrameImpl::LoadUrl(fidl::StringPtr url,
                        std::unique_ptr<chromium::web::LoadUrlParams> params) {
  GURL validated_url(*url);
  if (!validated_url.is_valid()) {
    DLOG(WARNING) << "Invalid URL: " << *url;
    return;
  }

  content::NavigationController::LoadURLParams params_converted(validated_url);
  params_converted.transition_type = ui::PageTransitionFromInt(
      ui::PAGE_TRANSITION_TYPED | ui::PAGE_TRANSITION_FROM_ADDRESS_BAR);
  web_contents_->GetController().LoadURLWithParams(params_converted);
 }
