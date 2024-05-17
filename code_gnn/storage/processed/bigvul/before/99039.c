PageInfoWindowGtk::PageInfoWindowGtk(gfx::NativeWindow parent,
                                     Profile* profile,
                                     const GURL& url,
                                     const NavigationEntry::SSLStatus& ssl,
                                     bool show_history)
    : ALLOW_THIS_IN_INITIALIZER_LIST(model_(profile, url, ssl,
                                            show_history, this)),
      url_(url),
      contents_(NULL),
      cert_id_(ssl.cert_id()) {
  dialog_ = gtk_dialog_new_with_buttons(
      l10n_util::GetStringUTF8(IDS_PAGEINFO_WINDOW_TITLE).c_str(),
      parent,
      GTK_DIALOG_NO_SEPARATOR,
      NULL);
  if (cert_id_) {
    gtk_dialog_add_button(
        GTK_DIALOG(dialog_),
        l10n_util::GetStringUTF8(IDS_PAGEINFO_CERT_INFO_BUTTON).c_str(),
        RESPONSE_SHOW_CERT_INFO);
  }
  gtk_dialog_add_button(GTK_DIALOG(dialog_), GTK_STOCK_CLOSE,
                        GTK_RESPONSE_CLOSE);
  gtk_dialog_set_default_response(GTK_DIALOG(dialog_), GTK_RESPONSE_CLOSE);

  gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(dialog_)->vbox),
                      gtk_util::kContentAreaSpacing);
  g_signal_connect(dialog_, "response", G_CALLBACK(OnDialogResponse), this);
  g_signal_connect(dialog_, "destroy", G_CALLBACK(OnDestroy), this);

  InitContents();

  g_page_info_window_map[url.spec()] = this;
}
