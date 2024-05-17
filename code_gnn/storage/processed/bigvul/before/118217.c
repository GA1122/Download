views::View* GetAncestralInputView(views::View* view) {
  if (view->GetClassName() == views::Combobox::kViewClassName)
    return view;

  return view->GetAncestorWithClassName(ExpandingTextfield::kViewClassName);
}
