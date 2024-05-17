void DiceResponseHandler::ProcessEnableSyncHeader(
    const std::string& gaia_id,
    const std::string& email,
    std::unique_ptr<ProcessDiceHeaderDelegate> delegate) {
  VLOG(1) << "Start processing Dice enable sync response";
  RecordDiceResponseHeader(kEnableSync);
  for (auto it = token_fetchers_.begin(); it != token_fetchers_.end(); ++it) {
    DiceTokenFetcher* fetcher = it->get();
    if (fetcher->gaia_id() == gaia_id) {
      DCHECK(gaia::AreEmailsSame(fetcher->email(), email));
      fetcher->set_should_enable_sync(true);
      return;   
    }
  }
  std::string account_id =
      account_tracker_service_->PickAccountIdForAccount(gaia_id, email);
  delegate->EnableSync(account_id);
}
