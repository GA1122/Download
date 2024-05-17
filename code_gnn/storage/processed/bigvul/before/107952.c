void LinkInfoBar::LinkActivated(views::Link* source, int event_flags) {
  DCHECK(source == link_);
  if (GetDelegate()->LinkClicked(
          event_utils::DispositionFromEventFlags(event_flags))) {
    RemoveInfoBar();
  }
}
