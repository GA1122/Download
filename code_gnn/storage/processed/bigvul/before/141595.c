std::string GetKeyForScriptContext(ScriptContext* script_context) {
  const std::string& extension_id = script_context->GetExtensionID();
  CHECK(crx_file::id_util::IdIsValid(extension_id) ||
        script_context->url().is_valid());
  return crx_file::id_util::IdIsValid(extension_id)
             ? extension_id
             : script_context->url().spec();
}
