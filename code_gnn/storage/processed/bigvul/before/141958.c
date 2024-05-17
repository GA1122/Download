AutofillPopupChildView* AutofillPopupViewViews::GetChildRow(
    size_t child_index) const {
  DCHECK_LT(child_index, static_cast<size_t>(child_count()));
  return static_cast<AutofillPopupChildView*>(
      const_cast<views::View*>(child_at(child_index)));
}
