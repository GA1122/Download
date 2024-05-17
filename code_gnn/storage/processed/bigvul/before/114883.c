void TestingAutomationProvider::FillAutofillProfile(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  ListValue* profiles = NULL;
  ListValue* cards = NULL;

  args->GetList("profiles", &profiles);
  args->GetList("credit_cards", &cards);

  std::string error_mesg;

  std::vector<AutofillProfile> autofill_profiles;
  std::vector<CreditCard> credit_cards;
  if (profiles) {
    autofill_profiles = GetAutofillProfilesFromList(*profiles, &error_mesg);
  }
  if (cards) {
    credit_cards = GetCreditCardsFromList(*cards, &error_mesg);
  }
  if (!error_mesg.empty()) {
    AutomationJSONReply(this, reply_message).SendError(error_mesg);
    return;
  }

  int tab_index = 0;
  if (!args->GetInteger("tab_index", &tab_index)) {
    AutomationJSONReply(this, reply_message).SendError(
        "Invalid or missing tab_index integer");
    return;
  }

  TabContentsWrapper* tab_contents =
      browser->GetTabContentsWrapperAt(tab_index);

  if (tab_contents) {
    PersonalDataManager* pdm =
        PersonalDataManagerFactory::GetForProfile(tab_contents->profile());
    if (pdm) {
      if (profiles || cards) {
        AutofillChangedObserver* observer = new AutofillChangedObserver(
            this, reply_message, autofill_profiles.size(), credit_cards.size());
        observer->Init();

        if (profiles)
          pdm->SetProfiles(&autofill_profiles);
        if (cards)
          pdm->SetCreditCards(&credit_cards);

        return;
      }
    } else {
      AutomationJSONReply(this, reply_message).SendError(
          "No PersonalDataManager.");
      return;
    }
  } else {
    AutomationJSONReply(this, reply_message).SendError("No tab at that index.");
    return;
  }
  AutomationJSONReply(this, reply_message).SendSuccess(NULL);
}
