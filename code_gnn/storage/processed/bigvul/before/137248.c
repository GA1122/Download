void Textfield::OnCaretBoundsChanged() {
  if (GetInputMethod())
    GetInputMethod()->OnCaretBoundsChanged(this);
  if (touch_selection_controller_)
    touch_selection_controller_->SelectionChanged();

#if defined(OS_MACOSX)
  context_menu_contents_.reset();
#endif

  if (HasFocus())
    NotifyAccessibilityEvent(ax::mojom::Event::kTextSelectionChanged, true);
}
