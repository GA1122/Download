void WebLocalFrameImpl::PerformMediaPlayerAction(
    const WebPoint& location,
    const WebMediaPlayerAction& action) {
  HitTestResult result = HitTestResultForVisualViewportPos(location);
  Node* node = result.InnerNode();
  if (!IsHTMLVideoElement(*node) && !IsHTMLAudioElement(*node))
    return;

  HTMLMediaElement* media_element = ToHTMLMediaElement(node);
  switch (action.type) {
    case WebMediaPlayerAction::kPlay:
      if (action.enable)
        media_element->Play();
      else
        media_element->pause();
      break;
    case WebMediaPlayerAction::kMute:
      media_element->setMuted(action.enable);
      break;
    case WebMediaPlayerAction::kLoop:
      media_element->SetLoop(action.enable);
      break;
    case WebMediaPlayerAction::kControls:
      media_element->SetBooleanAttribute(HTMLNames::controlsAttr,
                                         action.enable);
      break;
    case WebMediaPlayerAction::kPictureInPicture:
      DCHECK(media_element->IsHTMLVideoElement());
      if (action.enable) {
        PictureInPictureController::From(node->GetDocument())
            .EnterPictureInPicture(ToHTMLVideoElement(media_element), nullptr);
      } else {
        PictureInPictureController::From(node->GetDocument())
            .ExitPictureInPicture(ToHTMLVideoElement(media_element), nullptr);
      }
      break;
    default:
      NOTREACHED();
  }
}
