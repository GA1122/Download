bool CanonicalizeUserInfo(const base::char16* username_source,
                          const Component& username,
                          const base::char16* password_source,
                          const Component& password,
                          CanonOutput* output,
                          Component* out_username,
                          Component* out_password) {
  return DoUserInfo<base::char16, base::char16>(
      username_source, username, password_source, password,
      output, out_username, out_password);
}
