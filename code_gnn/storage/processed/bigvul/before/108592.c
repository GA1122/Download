gboolean Shell::OnNewWindowKeyPressed(GtkAccelGroup* accel_group,
                                      GObject* acceleratable,
                                      guint keyval,
                                      GdkModifierType modifier) {
  ShellBrowserContext* browser_context =
      static_cast<ShellContentBrowserClient*>(
        GetContentClient()->browser())->browser_context();
  Shell::CreateNewWindow(browser_context,
                         GURL(),
                         NULL,
                         MSG_ROUTING_NONE,
                         NULL);
  return TRUE;
}
