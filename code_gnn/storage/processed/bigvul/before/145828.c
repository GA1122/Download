std::unique_ptr<base::DictionaryValue> CreateInvalidParamResponse(
    int command_id,
    const std::string& param) {
  return CreateErrorResponse(
      command_id, kErrorInvalidParam,
      base::StringPrintf("Missing or invalid '%s' parameter", param.c_str()));
}
