void PageInfoWindowGtk::InitContents() {
  if (contents_)
    gtk_widget_destroy(contents_);
  contents_ = gtk_vbox_new(FALSE, gtk_util::kContentAreaSpacing);
  for (int i = 0; i < model_.GetSectionCount(); i++) {
    gtk_box_pack_start(GTK_BOX(contents_),
                       CreateSection(model_.GetSectionInfo(i)),
                       FALSE, FALSE, 0);
  }
  gtk_widget_show_all(contents_);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog_)->vbox), contents_);
}
