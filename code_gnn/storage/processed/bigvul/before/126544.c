CustomDrawButton* TabStripGtk::MakeNewTabButton() {
  CustomDrawButton* button = new CustomDrawButton(IDR_NEWTAB_BUTTON,
      IDR_NEWTAB_BUTTON_P, IDR_NEWTAB_BUTTON_H, 0);

  gtk_widget_set_tooltip_text(button->widget(),
      l10n_util::GetStringUTF8(IDS_TOOLTIP_NEW_TAB).c_str());

  gtk_util::SetButtonTriggersNavigation(button->widget());
  g_signal_connect(button->widget(), "clicked",
                   G_CALLBACK(OnNewTabClickedThunk), this);
  gtk_widget_set_can_focus(button->widget(), FALSE);
  gtk_fixed_put(GTK_FIXED(tabstrip_.get()), button->widget(), 0, 0);

  return button;
}
