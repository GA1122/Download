PasswordAutofillAgent::~PasswordAutofillAgent() {
  AutofillAgent* agent = autofill_agent_.get();
  if (agent)
    agent->RemoveFormObserver(this);
}
