views::View* AutofillDialogViews::SectionContainer::TargetForRect(
    views::View* root,
    const gfx::Rect& rect) {
  CHECK_EQ(root, this);
  views::View* handler = views::ViewTargeterDelegate::TargetForRect(root, rect);

  if (!background() &&
      rect.CenterPoint().y() > child_at(0)->bounds().bottom()) {
    return handler;
  }

  if (forward_mouse_events_ &&
      handler->GetAncestorWithClassName(ExpandingTextfield::kViewClassName)) {
    return handler;
  }

  if (handler == proxy_button_)
    return handler;

  return this;
}
