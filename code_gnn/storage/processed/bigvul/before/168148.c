void AutofillMetricsTest::RecreateFullServerCreditCardWithBankName() {
  personal_data_->ClearCreditCards();
  CreditCard credit_card(CreditCard::FULL_SERVER_CARD, "server_id");
  test::SetCreditCardInfo(&credit_card, "name", "4111111111111111", "12", "24",
                          "1");
  credit_card.set_guid("10000000-0000-0000-0000-000000000003");
  credit_card.set_bank_name("Chase");
  personal_data_->AddFullServerCreditCard(credit_card);
  personal_data_->Refresh();
}
