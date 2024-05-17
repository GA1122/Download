void TabStripGtk::OnNewTabClicked(GtkWidget* widget) {
  GdkEvent* event = gtk_get_current_event();
  DCHECK_EQ(event->type, GDK_BUTTON_RELEASE);
  int mouse_button = event->button.button;
  gdk_event_free(event);

  switch (mouse_button) {
    case 1:
      content::RecordAction(UserMetricsAction("NewTab_Button"));
      UMA_HISTOGRAM_ENUMERATION("Tab.NewTab", TabStripModel::NEW_TAB_BUTTON,
                                TabStripModel::NEW_TAB_ENUM_COUNT);
      model_->delegate()->AddBlankTabAt(-1, true);
      break;
    case 2: {
      GURL url;
      if (!gtk_util::URLFromPrimarySelection(model_->profile(), &url))
        return;

      Browser* browser = window_->browser();
      DCHECK(browser);
      chrome::AddSelectedTabWithURL(browser, url, content::PAGE_TRANSITION_TYPED);
      break;
    }
    default:
      NOTREACHED() << "Got click on new tab button with unhandled mouse "
                   << "button " << mouse_button;
  }
}
