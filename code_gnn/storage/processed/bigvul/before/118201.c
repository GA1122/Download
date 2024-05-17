void AutofillDialogViews::ContentsChanged(views::Textfield* sender,
                                          const base::string16& new_contents) {
  InputEditedOrActivated(TypeForTextfield(sender),
                         sender->GetBoundsInScreen(),
                         true);

  const ExpandingTextfield* expanding = static_cast<ExpandingTextfield*>(
      sender->GetAncestorWithClassName(ExpandingTextfield::kViewClassName));
  if (expanding && expanding->needs_layout())
    ContentsPreferredSizeChanged();
}
