void AutofillPopupViewViews::OnSelectedRowChanged(
    base::Optional<int> previous_row_selection,
    base::Optional<int> current_row_selection) {
  SchedulePaint();

  if (previous_row_selection) {
    GetChildRow(*previous_row_selection)->OnUnselected();
  }
  if (current_row_selection) {
    AutofillPopupChildView* current_row = GetChildRow(*current_row_selection);
    current_row->OnSelected();
    current_row->NotifyAccessibilityEvent(ax::mojom::Event::kSelection, true);
  }
}
