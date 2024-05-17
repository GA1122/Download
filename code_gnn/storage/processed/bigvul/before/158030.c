std::unique_ptr<WebMediaPlayer> LocalFrameClientImpl::CreateWebMediaPlayer(
    HTMLMediaElement& html_media_element,
    const WebMediaPlayerSource& source,
    WebMediaPlayerClient* client,
    WebLayerTreeView* layer_tree_view) {
  WebLocalFrameImpl* web_frame =
      WebLocalFrameImpl::FromFrame(html_media_element.GetDocument().GetFrame());

  if (!web_frame || !web_frame->Client())
    return nullptr;

  return CoreInitializer::GetInstance().CreateWebMediaPlayer(
      web_frame->Client(), html_media_element, source, client, layer_tree_view);
}
