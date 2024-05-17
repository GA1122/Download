CustomButton* CustomButton::AsCustomButton(views::View* view) {
  if (view) {
    const char* classname = view->GetClassName();
    if (!strcmp(classname, Checkbox::kViewClassName) ||
        !strcmp(classname, CustomButton::kViewClassName) ||
        !strcmp(classname, ImageButton::kViewClassName) ||
        !strcmp(classname, LabelButton::kViewClassName) ||
        !strcmp(classname, RadioButton::kViewClassName) ||
        !strcmp(classname, MenuButton::kViewClassName)) {
      return static_cast<CustomButton*>(view);
    }
  }
  return NULL;
}
