void PeopleHandler::HandleRequestPinLoginState(const base::ListValue* args) {
  AllowJavascript();
  chromeos::quick_unlock::PinBackend::GetInstance()->HasLoginSupport(
      base::BindOnce(&PeopleHandler::OnPinLoginAvailable,
                     weak_factory_.GetWeakPtr()));
}
