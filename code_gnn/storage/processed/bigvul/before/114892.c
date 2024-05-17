void TestingAutomationProvider::GetAutofillProfile(
    Browser* browser,
    DictionaryValue* args,
    IPC::Message* reply_message) {
  AutomationJSONReply reply(this, reply_message);
  int tab_index = 0;
  if (!args->GetInteger("tab_index", &tab_index)) {
    reply.SendError("Invalid or missing tab_index integer value.");
    return;
  }

  TabContentsWrapper* tab_contents =
      browser->GetTabContentsWrapperAt(tab_index);
  if (tab_contents) {
    PersonalDataManager* pdm = PersonalDataManagerFactory::GetForProfile(
        tab_contents->profile()->GetOriginalProfile());
    if (pdm) {
      std::vector<AutofillProfile*> autofill_profiles = pdm->profiles();
      std::vector<CreditCard*> credit_cards = pdm->credit_cards();

      ListValue* profiles = GetListFromAutofillProfiles(autofill_profiles);
      ListValue* cards = GetListFromCreditCards(credit_cards);

      scoped_ptr<DictionaryValue> return_value(new DictionaryValue);

      return_value->Set("profiles", profiles);
      return_value->Set("credit_cards", cards);
      reply.SendSuccess(return_value.get());
    } else {
      reply.SendError("No PersonalDataManager.");
      return;
    }
  } else {
    reply.SendError("No tab at that index.");
    return;
  }
}
