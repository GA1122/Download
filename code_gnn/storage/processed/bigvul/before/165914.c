void RenderFrameImpl::LoadHTMLString(const std::string& html,
                                     const GURL& base_url,
                                     const std::string& text_encoding,
                                     const GURL& unreachable_url,
                                     bool replace_current_item) {
  auto navigation_params = std::make_unique<WebNavigationParams>();
  navigation_params->request = WebURLRequest(base_url);
  navigation_params->data = WebData(html.data(), html.length());
  navigation_params->mime_type = "text/html";
  navigation_params->text_encoding = WebString::FromUTF8(text_encoding);
  navigation_params->unreachable_url = unreachable_url;
  navigation_params->frame_load_type =
      replace_current_item ? blink::WebFrameLoadType::kReplaceCurrentItem
                           : blink::WebFrameLoadType::kStandard;
  frame_->CommitNavigation(std::move(navigation_params),
                           nullptr  );
}
