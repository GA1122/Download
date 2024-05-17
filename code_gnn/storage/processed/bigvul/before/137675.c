void PrintPreviewDialogController::OnNavEntryCommitted(
    WebContents* contents, content::LoadCommittedDetails* details) {
  WebContents* preview_dialog = GetPrintPreviewForContents(contents);
  if (!preview_dialog) {
    NOTREACHED();
    return;
  }

  if (contents == preview_dialog) {
    if (details) {
      ui::PageTransition transition_type =
          details->entry->GetTransitionType();
      content::NavigationType nav_type = details->type;

      if (waiting_for_new_preview_page_ &&
          ui::PageTransitionCoreTypeIs(transition_type,
                                       ui::PAGE_TRANSITION_AUTO_TOPLEVEL) &&
          nav_type == content::NAVIGATION_TYPE_NEW_PAGE) {
        waiting_for_new_preview_page_ = false;
        SaveInitiatorTitle(preview_dialog);
        return;
      }

      if (!waiting_for_new_preview_page_ &&
          ui::PageTransitionCoreTypeIs(transition_type,
                                       ui::PAGE_TRANSITION_RELOAD) &&
          nav_type == content::NAVIGATION_TYPE_EXISTING_PAGE &&
          IsPrintPreviewURL(details->previous_url)) {
        return;
      }
    }
    NOTREACHED();
    return;
  }
  if (details) {
    ui::PageTransition type = details->entry->GetTransitionType();
    content::NavigationType nav_type = details->type;
    if (nav_type == content::NAVIGATION_TYPE_EXISTING_PAGE &&
        (ui::PageTransitionTypeIncludingQualifiersIs(
             type,
             ui::PageTransitionFromInt(ui::PAGE_TRANSITION_TYPED |
                                       ui::PAGE_TRANSITION_FROM_ADDRESS_BAR)) ||
         ui::PageTransitionTypeIncludingQualifiersIs(type,
                                                     ui::PAGE_TRANSITION_LINK)))
      return;
  }

  RemoveInitiator(contents);
}
