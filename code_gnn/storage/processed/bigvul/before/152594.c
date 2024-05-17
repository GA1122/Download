void TestRenderFrame::BeginNavigation(
    std::unique_ptr<blink::WebNavigationInfo> info) {
  if (next_navigation_html_override_.has_value()) {
    auto navigation_params = blink::WebNavigationParams::CreateWithHTMLString(
        next_navigation_html_override_.value(), info->url_request.Url());
    next_navigation_html_override_ = base::nullopt;
    frame_->CommitNavigation(std::move(navigation_params),
                             nullptr  );
    return;
  }
  if (info->navigation_policy == blink::kWebNavigationPolicyCurrentTab &&
      GetWebFrame()->Parent() && info->form.IsNull()) {
    GURL url = info->url_request.Url();
    auto navigation_params = std::make_unique<blink::WebNavigationParams>();
    navigation_params->url = url;
    if (!url.IsAboutBlank() && url != content::kAboutSrcDocURL) {
      std::string mime_type, charset, data;
      if (!net::DataURL::Parse(url, &mime_type, &charset, &data)) {
        mime_type = "text/html";
        charset = "UTF-8";
      }
      blink::WebNavigationParams::FillStaticResponse(
          navigation_params.get(), blink::WebString::FromUTF8(mime_type),
          blink::WebString::FromUTF8(charset), data);
    }
    frame_->CommitNavigation(std::move(navigation_params),
                             nullptr  );
    return;
  }
  RenderFrameImpl::BeginNavigation(std::move(info));
}
