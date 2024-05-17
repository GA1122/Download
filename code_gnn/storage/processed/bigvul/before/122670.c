bool Extension::LoadBackgroundAllowJSAccess(
    const APIPermissionSet& api_permissions,
    string16* error) {
  Value* allow_js_access = NULL;
  if (!manifest_->Get(keys::kBackgroundAllowJsAccess, &allow_js_access))
    return true;

  if (!allow_js_access->IsType(Value::TYPE_BOOLEAN) ||
      !allow_js_access->GetAsBoolean(&allow_background_js_access_)) {
    *error = ASCIIToUTF16(errors::kInvalidBackgroundAllowJsAccess);
    return false;
  }

  return true;
}
