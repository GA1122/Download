static bool CanUpcastTo(const PropertyTreeState& guest,
                        const PropertyTreeState& home) {
  DCHECK_EQ(home.Effect(), guest.Effect());

  if (IsBackfaceHidden(home.Transform()) != IsBackfaceHidden(guest.Transform()))
    return false;

  for (const ClipPaintPropertyNode* current_clip = guest.Clip();
       current_clip != home.Clip(); current_clip = current_clip->Parent()) {
    if (!current_clip || current_clip->HasDirectCompositingReasons())
      return false;
    if (!IsNonCompositingAncestorOf(home.Transform(),
                                    current_clip->LocalTransformSpace()))
      return false;
  }

  return IsNonCompositingAncestorOf(home.Transform(), guest.Transform());
}
