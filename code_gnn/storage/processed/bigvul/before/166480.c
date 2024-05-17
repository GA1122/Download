bool RunRequestFunction(
    const Extension& extension,
    Browser* browser,
    const char* args,
    std::unique_ptr<const PermissionSet>* prompted_permissions_out) {
  auto function = base::MakeRefCounted<PermissionsRequestFunction>();
  function->set_user_gesture(true);
  function->set_extension(&extension);
  std::unique_ptr<base::Value> result(
      extension_function_test_utils::RunFunctionAndReturnSingleResult(
          function.get(), args, browser, api_test_utils::NONE));
  if (!function->GetError().empty()) {
    ADD_FAILURE() << "Unexpected function error: " << function->GetError();
    return false;
  }

  if (!result || !result->is_bool()) {
    ADD_FAILURE() << "Unexpected function result.";
    return false;
  }

  *prompted_permissions_out = function->TakePromptedPermissionsForTesting();

  return result->GetBool();
}
