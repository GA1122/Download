  std::string RunCreateWindowExpectError(const std::string& args) {
    scoped_refptr<WindowsCreateFunction> function(new WindowsCreateFunction);
    function->set_extension(ExtensionBuilder("Test").Build().get());
    return api_test_utils::RunFunctionAndReturnError(function.get(), args,
                                                     browser()->profile());
  }
