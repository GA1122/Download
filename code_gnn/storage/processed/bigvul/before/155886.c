void ProfileChooserView::EnableSync(
    const base::Optional<AccountInfo>& account) {
  Hide();
  if (account)
    signin_ui_util::EnableSyncFromPromo(browser_, account.value(),
                                        access_point_,
                                        false  );
  else
    ShowViewFromMode(profiles::BUBBLE_VIEW_MODE_GAIA_SIGNIN);
}
