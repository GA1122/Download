ExtensionFunction::ResponseAction TabsCreateFunction::Run() {
  std::unique_ptr<tabs::Create::Params> params(
      tabs::Create::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  ExtensionTabUtil::OpenTabParams options;
  AssignOptionalValue(params->create_properties.window_id, &options.window_id);
  AssignOptionalValue(params->create_properties.opener_tab_id,
                      &options.opener_tab_id);
  AssignOptionalValue(params->create_properties.selected, &options.active);
  AssignOptionalValue(params->create_properties.active, &options.active);
  AssignOptionalValue(params->create_properties.pinned, &options.pinned);
  AssignOptionalValue(params->create_properties.index, &options.index);
  AssignOptionalValue(params->create_properties.url, &options.url);

  std::string error;
  std::unique_ptr<base::DictionaryValue> result(
      ExtensionTabUtil::OpenTab(this, options, user_gesture(), &error));
  if (!result)
    return RespondNow(Error(error));

  return RespondNow(has_callback() ? OneArgument(std::move(result))
                                   : NoArguments());
}
