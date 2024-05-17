base::string16 AutofillDialogViews::GetWindowTitle() const {
  base::string16 title = delegate_->DialogTitle();
  return title.empty() ? base::ASCIIToUTF16(" ") : title;
}
