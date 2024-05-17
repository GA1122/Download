void AutofillMetricsTest::RecreateCreditCards(
    bool include_local_credit_card,
    bool include_masked_server_credit_card,
    bool include_full_server_credit_card) {
  personal_data_->ClearCreditCards();
  if (include_local_credit_card) {
    CreditCard local_credit_card;
    test::SetCreditCardInfo(&local_credit_card, "Test User",
                            "4111111111111111"  , "11", "2022", "1");
    local_credit_card.set_guid("10000000-0000-0000-0000-000000000001");
    personal_data_->AddCreditCard(local_credit_card);
  }
  if (include_masked_server_credit_card) {
    CreditCard masked_server_credit_card(CreditCard::MASKED_SERVER_CARD,
                                         "server_id");
    masked_server_credit_card.set_guid("10000000-0000-0000-0000-000000000002");
    masked_server_credit_card.SetNetworkForMaskedCard(kDiscoverCard);
    masked_server_credit_card.SetNumber(ASCIIToUTF16("9424"));
    personal_data_->AddServerCreditCard(masked_server_credit_card);
  }
  if (include_full_server_credit_card) {
    CreditCard full_server_credit_card(CreditCard::FULL_SERVER_CARD,
                                       "server_id");
    full_server_credit_card.set_guid("10000000-0000-0000-0000-000000000003");
    personal_data_->AddFullServerCreditCard(full_server_credit_card);
  }
  personal_data_->Refresh();
}
