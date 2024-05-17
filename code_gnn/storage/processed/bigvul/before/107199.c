  void CreateTestCreditCards(ScopedVector<CreditCard>* credit_cards) {
    CreditCard* credit_card = new CreditCard;
    autofill_test::SetCreditCardInfo(credit_card, "First", "Elvis Presley",
                                     "4234567890123456",  
                                     "04", "2012");
    credit_card->set_guid("00000000-0000-0000-0000-000000000004");
    credit_cards->push_back(credit_card);
    credit_card = new CreditCard;
    autofill_test::SetCreditCardInfo(credit_card, "Second", "Buddy Holly",
                                     "5187654321098765",  
                                     "10", "2014");
    credit_card->set_guid("00000000-0000-0000-0000-000000000005");
    credit_cards->push_back(credit_card);
    credit_card = new CreditCard;
    autofill_test::SetCreditCardInfo(credit_card, "Empty", "", "", "", "");
    credit_card->set_guid("00000000-0000-0000-0000-000000000006");
    credit_cards->push_back(credit_card);
  }
