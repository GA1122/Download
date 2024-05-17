void AutofillPopupViewViews::CreateChildViews() {
  RemoveAllChildViews(true  );

  int set_size = controller_->GetLineCount();
  for (int i = 0; i < set_size; ++i) {
    AddChildView(new AutofillPopupChildView(controller_->GetSuggestionAt(i),
                                            set_size, i + 1));
  }
}
