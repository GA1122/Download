blink::WebMediaPlayer* RenderFrameImpl::CreateMediaPlayer(
    const blink::WebMediaPlayerSource& source,
    WebMediaPlayerClient* client,
    WebMediaPlayerEncryptedMediaClient* encrypted_client,
    WebContentDecryptionModule* initial_cdm,
    const blink::WebString& sink_id,
    blink::WebLayerTreeView* layer_tree_view) {
  const cc::LayerTreeSettings& settings =
      GetRenderWidget()->layer_tree_view()->GetLayerTreeSettings();
  return media_factory_.CreateMediaPlayer(source, client, encrypted_client,
                                          initial_cdm, sink_id, layer_tree_view,
                                          settings);
}
