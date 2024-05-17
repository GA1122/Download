std::unique_ptr<base::DictionaryValue> CreateErrorResponse(
    int command_id,
    int error_code,
    const std::string& error_message) {
  auto error_object = std::make_unique<base::DictionaryValue>();
  error_object->SetInteger(kErrorCodeParam, error_code);
  error_object->SetString(kErrorMessageParam, error_message);

  auto response = std::make_unique<base::DictionaryValue>();
  response->SetInteger(kIdParam, command_id);
  response->Set(kErrorParam, std::move(error_object));
  return response;
}
