ExtensionFunction::ResponseAction TabsRemoveFunction::Run() {
  std::unique_ptr<tabs::Remove::Params> params(
      tabs::Remove::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  std::string error;
  if (params->tab_ids.as_integers) {
    std::vector<int>& tab_ids = *params->tab_ids.as_integers;
    for (size_t i = 0; i < tab_ids.size(); ++i) {
      if (!RemoveTab(tab_ids[i], &error))
        return RespondNow(Error(error));
    }
  } else {
    EXTENSION_FUNCTION_VALIDATE(params->tab_ids.as_integer);
    if (!RemoveTab(*params->tab_ids.as_integer, &error))
      return RespondNow(Error(error));
  }
  return RespondNow(NoArguments());
}
