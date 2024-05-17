void AutomationProviderGetPasswordsObserver::OnPasswordStoreRequestDone(
    CancelableRequestProvider::Handle handle,
    const std::vector<content::PasswordForm*>& result) {
  if (!provider_) {
    delete this;
    return;
  }

  scoped_ptr<DictionaryValue> return_value(new DictionaryValue);

  ListValue* passwords = new ListValue;
  for (std::vector<content::PasswordForm*>::const_iterator it =
           result.begin(); it != result.end(); ++it) {
    DictionaryValue* password_val = new DictionaryValue;
    content::PasswordForm* password_form = *it;
    password_val->SetString("username_value", password_form->username_value);
    password_val->SetString("password_value", password_form->password_value);
    password_val->SetString("signon_realm", password_form->signon_realm);
    password_val->SetDouble(
        "time", static_cast<double>(password_form->date_created.ToDoubleT()));
    password_val->SetString("origin_url", password_form->origin.spec());
    password_val->SetString("username_element",
                            password_form->username_element);
    password_val->SetString("password_element",
                            password_form->password_element);
    password_val->SetString("submit_element", password_form->submit_element);
    password_val->SetString("action_target", password_form->action.spec());
    password_val->SetBoolean("blacklist", password_form->blacklisted_by_user);
    passwords->Append(password_val);
  }

  return_value->Set("passwords", passwords);
  AutomationJSONReply(provider_, reply_message_.release()).SendSuccess(
      return_value.get());
  delete this;
}
