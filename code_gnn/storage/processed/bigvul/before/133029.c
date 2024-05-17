void RenderWidgetHostViewAura::SetParentNativeViewAccessible(
    gfx::NativeViewAccessible accessible_parent) {
  if (GetBrowserAccessibilityManager()) {
    GetBrowserAccessibilityManager()->ToBrowserAccessibilityManagerWin()
        ->set_parent_iaccessible(accessible_parent);
  }
}
