Browser* Browser::CreateForType(Type type, Profile* profile) {
  CreateParams params(type, profile);
  return CreateWithParams(params);
}
