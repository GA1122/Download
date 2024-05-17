  const Extension* StartTestFromBackgroundPage(const char* script_name,
                                               std::string* error_or_null) {
    const Extension* extension =
        LoadExtension(test_data_dir_.AppendASCII("service_worker/background"));
    CHECK(extension);
    ExtensionHost* background_host =
        process_manager()->GetBackgroundHostForExtension(extension->id());
    CHECK(background_host);
    std::string error;
    CHECK(content::ExecuteScriptAndExtractString(
        background_host->host_contents(),
        base::StringPrintf("test.registerServiceWorker('%s')", script_name),
        &error));
    if (error_or_null)
      *error_or_null = error;
    else if (!error.empty())
      ADD_FAILURE() << "Got unexpected error " << error;
    return extension;
  }
