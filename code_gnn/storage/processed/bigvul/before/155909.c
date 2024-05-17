void ProfileChooserView::ResetView() {
  open_other_profile_indexes_map_.clear();
  delete_account_button_map_.clear();
  reauth_account_button_map_.clear();
  sync_error_button_ = nullptr;
  manage_accounts_link_ = nullptr;
  manage_accounts_button_ = nullptr;
  signin_current_profile_button_ = nullptr;
  signin_with_gaia_account_button_ = nullptr;
  current_profile_card_ = nullptr;
  first_profile_button_ = nullptr;
  guest_profile_button_ = nullptr;
  users_button_ = nullptr;
  go_incognito_button_ = nullptr;
  lock_button_ = nullptr;
  close_all_windows_button_ = nullptr;
  add_account_link_ = nullptr;
  gaia_signin_cancel_button_ = nullptr;
  remove_account_button_ = nullptr;
  account_removal_cancel_button_ = nullptr;
  sync_to_another_account_button_ = nullptr;
  dice_signin_button_view_ = nullptr;
  passwords_button_ = nullptr;
  credit_cards_button_ = nullptr;
  addresses_button_ = nullptr;
  signout_button_ = nullptr;
}
