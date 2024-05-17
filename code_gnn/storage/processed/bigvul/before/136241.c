 int GetValidatorOptions(Extension* extension) {
   int options = csp_validator::OPTIONS_NONE;

  if (extension->GetType() == Manifest::TYPE_EXTENSION ||
      extension->GetType() == Manifest::TYPE_LEGACY_PACKAGED_APP) {
    options |= csp_validator::OPTIONS_ALLOW_UNSAFE_EVAL;
  }

  if (extensions::Manifest::IsComponentLocation(extension->location()))
    options |= csp_validator::OPTIONS_ALLOW_INSECURE_OBJECT_SRC;

  return options;
}
