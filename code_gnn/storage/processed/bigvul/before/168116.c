  void CreateTestServerAndLocalCreditCards() {
    personal_data_.ClearCreditCards();

    CreditCard masked_server_card;
    test::SetCreditCardInfo(&masked_server_card, "Elvis Presley",
                            "4234567890123456",   
                            "04", "2999", "1");
    masked_server_card.set_guid("00000000-0000-0000-0000-000000000007");
    masked_server_card.set_record_type(CreditCard::MASKED_SERVER_CARD);
    personal_data_.AddServerCreditCard(masked_server_card);

    CreditCard full_server_card;
    test::SetCreditCardInfo(&full_server_card, "Buddy Holly",
                            "5187654321098765",   
                            "10", "2998", "1");
    full_server_card.set_guid("00000000-0000-0000-0000-000000000008");
    full_server_card.set_record_type(CreditCard::FULL_SERVER_CARD);
    personal_data_.AddServerCreditCard(full_server_card);

    CreditCard local_card;
    test::SetCreditCardInfo(&local_card, "Elvis Presley",
                            "4234567890123456",   
                            "04", "2999", "1");
    local_card.set_guid("00000000-0000-0000-0000-000000000009");
    local_card.set_record_type(CreditCard::LOCAL_CARD);
    personal_data_.AddCreditCard(local_card);
  }
