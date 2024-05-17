void ConfirmInfoBar::LinkClicked(views::Link* source, int event_flags) {
  if (!owner())
    return;   
  DCHECK_EQ(link_, source);
  if (GetDelegate()->LinkClicked(ui::DispositionFromEventFlags(event_flags)))
    RemoveSelf();
}
