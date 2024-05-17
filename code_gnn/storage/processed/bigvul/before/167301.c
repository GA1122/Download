void ScrollableShelfView::OnFocusRingActivationChanged(bool activated) {
  if (activated) {
    focus_ring_activated_ = true;
    SetPaneFocusAndFocusDefault();

    GradientLayerDelegate::FadeZone fade_zone = {gfx::Rect(), false, false};
    gradient_layer_delegate_->set_fade_zone(fade_zone);
    gradient_layer_delegate_->layer()->SetBounds(layer()->bounds());
    SchedulePaint();
  } else {
    focus_ring_activated_ = false;
    UpdateGradientZone();
  }
}
