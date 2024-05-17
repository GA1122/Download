bool Tab::GetHitTestMask(SkPath* mask) const {
  *mask = tab_style()->GetPath(
      TabStyle::PathType::kHitTest,
      GetWidget()->GetCompositor()->device_scale_factor(),
        false, TabStyle::RenderUnits::kDips);
  return true;
}
