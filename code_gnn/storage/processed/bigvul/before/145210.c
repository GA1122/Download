void Dispatcher::DidCreateDocumentElement(blink::WebLocalFrame* frame) {
  GURL effective_document_url = ScriptContext::GetEffectiveDocumentURL(
      frame, frame->document().url(), true  );

  const Extension* extension =
      RendererExtensionRegistry::Get()->GetExtensionOrAppByURL(
          effective_document_url);

  if (extension &&
      (extension->is_extension() || extension->is_platform_app())) {
    int resource_id = extension->is_platform_app() ? IDR_PLATFORM_APP_CSS
                                                   : IDR_EXTENSION_FONTS_CSS;
    std::string stylesheet = ResourceBundle::GetSharedInstance()
                                 .GetRawDataResource(resource_id)
                                 .as_string();
    base::ReplaceFirstSubstringAfterOffset(
        &stylesheet, 0, "$FONTFAMILY", system_font_family_);
    base::ReplaceFirstSubstringAfterOffset(
        &stylesheet, 0, "$FONTSIZE", system_font_size_);

    frame->document().insertStyleSheet(WebString::fromUTF8(stylesheet));
  }

  if (extension && extension->is_extension() &&
      OptionsPageInfo::ShouldUseChromeStyle(extension) &&
      effective_document_url == OptionsPageInfo::GetOptionsPage(extension)) {
    frame->document().insertStyleSheet(
        WebString::fromUTF8(ResourceBundle::GetSharedInstance()
                                .GetRawDataResource(IDR_EXTENSION_CSS)
                                .as_string()));
  }

  if (content_watcher_) {
    content_watcher_->DidCreateDocumentElement(frame);
  }
}
