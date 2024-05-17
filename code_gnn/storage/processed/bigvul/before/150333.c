void PasswordAutofillAgent::SetAutofillAgent(AutofillAgent* autofill_agent) {
  AutofillAgent* agent = autofill_agent_.get();
  if (agent)
    agent->RemoveFormObserver(this);
  autofill_agent_ = autofill_agent->GetWeakPtr();
  autofill_agent->AddFormObserver(this);
}
