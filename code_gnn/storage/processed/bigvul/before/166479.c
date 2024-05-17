  bool RunContainsFunction(const std::string& manifest_permission,
                           const std::string& args_string,
                           bool allow_file_access) {
    SCOPED_TRACE(args_string);
    ListBuilder required_permissions;
    required_permissions.Append(manifest_permission);
    scoped_refptr<const Extension> extension = CreateExtensionWithPermissions(
        required_permissions.Build(), "My Extension", allow_file_access);
    scoped_refptr<PermissionsContainsFunction> function(
        new PermissionsContainsFunction());
    function->set_extension(extension.get());
    bool run_result = extension_function_test_utils::RunFunction(
        function.get(), args_string, browser(), api_test_utils::NONE);
    EXPECT_TRUE(run_result) << function->GetError();

    bool has_permission;
    EXPECT_TRUE(function->GetResultList()->GetBoolean(0u, &has_permission));
    return has_permission;
  }
