bool HostNPScriptObject::LocalizeString(NPObject* localize_func,
                                        const char* tag, string16* result) {
  NPVariant args[2];
  STRINGZ_TO_NPVARIANT(tag, args[0]);
  NPVariant np_result;
  bool is_good = g_npnetscape_funcs->invokeDefault(
      plugin_, localize_func, &args[0], 1, &np_result);
  if (!is_good) {
    LOG(ERROR) << "Localization failed for " << tag;
    return false;
  }
  std::string translation = StringFromNPVariant(np_result);
  g_npnetscape_funcs->releasevariantvalue(&np_result);
  if (translation.empty()) {
    LOG(ERROR) << "Missing translation for " << tag;
    return false;
  }
  *result = UTF8ToUTF16(translation);
  return true;
}
