ExtensionFunction::ResponseAction TabsMoveFunction::Run() {
  std::unique_ptr<tabs::Move::Params> params(
      tabs::Move::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int new_index = params->move_properties.index;
  int* window_id = params->move_properties.window_id.get();
  std::unique_ptr<base::ListValue> tab_values(new base::ListValue());

  size_t num_tabs = 0;
  std::string error;
  if (params->tab_ids.as_integers) {
    std::vector<int>& tab_ids = *params->tab_ids.as_integers;
    num_tabs = tab_ids.size();
    for (size_t i = 0; i < tab_ids.size(); ++i) {
      if (!MoveTab(tab_ids[i], &new_index, i, tab_values.get(), window_id,
                   &error)) {
        return RespondNow(Error(error));
      }
    }
  } else {
    EXTENSION_FUNCTION_VALIDATE(params->tab_ids.as_integer);
    num_tabs = 1;
    if (!MoveTab(*params->tab_ids.as_integer, &new_index, 0, tab_values.get(),
                 window_id, &error)) {
      return RespondNow(Error(error));
    }
  }

  if (!has_callback())
    return RespondNow(NoArguments());

  if (num_tabs == 0)
    return RespondNow(Error("No tabs given."));
  if (num_tabs == 1) {
    std::unique_ptr<base::Value> value;
    CHECK(tab_values->Remove(0, &value));
    return RespondNow(OneArgument(std::move(value)));
  }

  return RespondNow(OneArgument(std::move(tab_values)));
}
