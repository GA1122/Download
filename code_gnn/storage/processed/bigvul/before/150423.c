bool IsWidgetPinned(views::Widget* widget) {
  ash::WindowPinType type =
      widget->GetNativeWindow()->GetProperty(ash::kWindowPinTypeKey);
  return type == ash::WindowPinType::kPinned ||
         type == ash::WindowPinType::kTrustedPinned;
}
