void HTMLMediaElement::ParseAttribute(
    const AttributeModificationParams& params) {
  const QualifiedName& name = params.name;
  if (name == srcAttr) {
    BLINK_MEDIA_LOG << "parseAttribute(" << (void*)this
                    << ", srcAttr, old=" << params.old_value
                    << ", new=" << params.new_value << ")";
    if (!params.new_value.IsNull()) {
      ignore_preload_none_ = false;
      InvokeLoadAlgorithm();
    }
  } else if (name == controlsAttr) {
    UseCounter::Count(GetDocument(),
                      WebFeature::kHTMLMediaElementControlsAttribute);
    UpdateControlsVisibility();
  } else if (name == controlslistAttr) {
    UseCounter::Count(GetDocument(),
                      WebFeature::kHTMLMediaElementControlsListAttribute);
    if (params.old_value != params.new_value) {
      controls_list_->DidUpdateAttributeValue(params.old_value,
                                              params.new_value);
      if (GetMediaControls())
        GetMediaControls()->OnControlsListUpdated();
    }
  } else if (name == preloadAttr) {
    SetPlayerPreload();
  } else if (name == disableremoteplaybackAttr) {
    UseCounter::Count(GetDocument(),
                      WebFeature::kDisableRemotePlaybackAttribute);
    if (params.old_value != params.new_value) {
      if (web_media_player_) {
        web_media_player_->RequestRemotePlaybackDisabled(
            !params.new_value.IsNull());
      }
    }
  } else {
    HTMLElement::ParseAttribute(params);
  }
}
