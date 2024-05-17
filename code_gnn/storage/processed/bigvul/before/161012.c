void ChromeClientImpl::ShowMouseOverURL(const HitTestResult& result) {
  if (!web_view_->Client())
    return;

  WebURL url;

  if (!result.GetScrollbar()) {
    if (result.IsLiveLink() &&
        !result.AbsoluteLinkURL().GetString().IsEmpty()) {
      url = result.AbsoluteLinkURL();
    } else if (result.InnerNode() &&
               (IsHTMLObjectElement(*result.InnerNode()) ||
                IsHTMLEmbedElement(*result.InnerNode()))) {
      LayoutObject* object = result.InnerNode()->GetLayoutObject();
      if (object && object->IsLayoutEmbeddedContent()) {
        WebPluginContainerImpl* plugin_view =
            ToLayoutEmbeddedContent(object)->Plugin();
        if (plugin_view) {
          url = plugin_view->Plugin()->LinkAtPosition(
              result.RoundedPointInInnerNodeFrame());
        }
      }
    }
  }

  web_view_->Client()->SetMouseOverURL(url);
}
