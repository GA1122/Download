void ExtensionInstalledBubbleGtk::OnButtonClick(GtkWidget* button,
    ExtensionInstalledBubbleGtk* bubble) {
  if (button == bubble->close_button_->widget()) {
    bubble->bubble_->Close();
  } else {
    NOTREACHED();
  }
}
