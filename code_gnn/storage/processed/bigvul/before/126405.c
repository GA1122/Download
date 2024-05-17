void BrowserWindowGtk::OnMainWindowDestroy(GtkWidget* widget) {
  extension_keybinding_registry_.reset();

  MessageLoop::current()->PostTask(
      FROM_HERE, base::Bind(&base::DeletePointer<BrowserWindowGtk>, this));
}
