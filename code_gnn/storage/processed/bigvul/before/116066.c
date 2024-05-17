void ExtensionSettingsHandler::HandleLoadMessage(const ListValue* args) {
  FilePath::StringType string_path;
  CHECK_EQ(1U, args->GetSize()) << args->GetSize();
  CHECK(args->GetString(0, &string_path));
  extensions::UnpackedInstaller::Create(extension_service_)->
      Load(FilePath(string_path));
}
