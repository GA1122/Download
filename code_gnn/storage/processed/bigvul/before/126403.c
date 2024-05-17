void BrowserWindowGtk::OnLocationIconSizeAllocate(GtkWidget* sender,
                                                  GtkAllocation* allocation) {
  InvalidateInfoBarBits();
}
