  MockAutofillManager(AutofillDriver* driver, MockAutofillClient* client)
      : AutofillManager(driver,
                        client,
                        client->GetPersonalDataManager(),
                        client->GetAutocompleteHistoryManager()) {}
