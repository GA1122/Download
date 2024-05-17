void RenderFrameImpl::LoadErrorPage(int reason) {
  WebURLError error(reason, frame_->GetDocument().Url());

  std::string error_html;
  GetContentClient()->renderer()->PrepareErrorPage(
      this, frame_->GetDocumentLoader()->GetRequest(), error, &error_html,
      nullptr);

  frame_->LoadData(error_html, WebString::FromUTF8("text/html"),
                   WebString::FromUTF8("UTF-8"), GURL(kUnreachableWebDataURL),
                   error.url(), true, blink::WebFrameLoadType::kStandard,
                   blink::WebHistoryItem(),
                   blink::kWebHistoryDifferentDocumentLoad, true);
}
