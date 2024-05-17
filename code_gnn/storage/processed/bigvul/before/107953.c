void ConfirmInfoBar::LinkActivated(views::Link* source, int event_flags) {
  DCHECK(source == link_);
  DCHECK(link_->IsVisible());
  DCHECK(!link_->GetText().empty());
  if (GetDelegate()->LinkClicked(
          event_utils::DispositionFromEventFlags(event_flags))) {
    RemoveInfoBar();
  }
}