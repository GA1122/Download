void AutofillDialogViews::StyledLabelLinkClicked(const gfx::Range& range,
                                                 int event_flags) {
  delegate_->LegalDocumentLinkClicked(range);
}
