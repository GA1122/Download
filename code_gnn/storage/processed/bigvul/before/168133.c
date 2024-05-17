  void PrepareForRealPanResponse(FormData* form, CreditCard* card) {
    base::CommandLine::ForCurrentProcess()->AppendSwitchASCII(
        "sync-url", "https://google.com");

    CreateTestCreditCardFormData(form, true, false);
    FormsSeen(std::vector<FormData>(1, *form));
    *card = CreditCard(CreditCard::MASKED_SERVER_CARD, "a123");
    test::SetCreditCardInfo(card, "John Dillinger", "1881"  , "01",
                            "2017", "1");
    card->SetNetworkForMaskedCard(kVisaCard);

    EXPECT_CALL(*autofill_driver_, SendFormDataToRenderer(_, _, _))
        .Times(AtLeast(1));
    autofill_manager_->FillOrPreviewCreditCardForm(
        AutofillDriver::FORM_DATA_ACTION_FILL, kDefaultPageID, *form,
        form->fields[0], *card);
  }
