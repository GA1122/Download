std::unique_ptr<base::DictionaryValue> CreateSuccessResponse(
    int command_id,
    std::unique_ptr<base::Value> result) {
  if (!result)
    result = std::make_unique<base::DictionaryValue>();

  auto response = std::make_unique<base::DictionaryValue>();
  response->SetInteger(kIdParam, command_id);
  response->Set(kResultParam, std::move(result));
  return response;
}
