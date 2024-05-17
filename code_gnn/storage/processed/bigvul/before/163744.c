void ChromeClientImpl::ShowMouseOverURL(const HitTestResult& result) {
  if (!web_view_->Client())
    return;

  WebURL url;

  if (!result.GetScrollbar()) {
    if (result.IsLiveLink() &&
        !result.AbsoluteLinkURL().GetString().IsEmpty()) {
      url = result.AbsoluteLinkURL();
    } else if (result.InnerNode() &&
               (isHTMLObjectElement(*result.InnerNode()) ||
                isHTMLEmbedElement(*result.InnerNode()))) {
      LayoutObject* object = result.InnerNode()->GetLayoutObject();
      if (object && object->IsLayoutEmbeddedContent()) {
        PluginView* plugin_view = ToLayoutEmbeddedContent(object)->Plugin();
        if (plugin_view && plugin_view->IsPluginContainer()) {
          WebPluginContainerImpl* plugin =
              ToWebPluginContainerImpl(plugin_view);
          url = plugin->Plugin()->LinkAtPosition(
              result.RoundedPointInInnerNodeFrame());
        }
      }
    }
  }

  web_view_->Client()->SetMouseOverURL(url);
}
