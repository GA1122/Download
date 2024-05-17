bool IsEnvironmentOptionEnabled(base::StringPiece option,
                                base::Environment* env) {
  std::string name("OXIDE_");
  name += option.data();

  std::string result;
  if (!env->GetVar(name, &result)) {
    return false;
  }

  return result.size() > 0 && result[0] != '0';
}
