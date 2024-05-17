std::string GetEnvironmentOption(base::StringPiece option,
                                 base::Environment* env) {
  std::string name("OXIDE_");
  name += option.data();

  std::string result;
  env->GetVar(name, &result);

  return result;
}
