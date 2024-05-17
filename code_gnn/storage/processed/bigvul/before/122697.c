bool Extension::LoadRequirements(string16* error) {
  ListValue* list_value = NULL;
  requirements_.npapi =
    manifest_->GetList(keys::kPlugins, &list_value) && !list_value->empty();

  if (!manifest_->HasKey(keys::kRequirements))
    return true;

  DictionaryValue* requirements_value = NULL;
  if (!manifest_->GetDictionary(keys::kRequirements, &requirements_value)) {
    *error = ASCIIToUTF16(errors::kInvalidRequirements);
    return false;
  }

  for (DictionaryValue::Iterator it(*requirements_value); !it.IsAtEnd();
       it.Advance()) {
    const DictionaryValue* requirement_value;
    if (!it.value().GetAsDictionary(&requirement_value)) {
      *error = ErrorUtils::FormatErrorMessageUTF16(
          errors::kInvalidRequirement, it.key());
      return false;
    }

    if (it.key() == "plugins") {
      for (DictionaryValue::Iterator plugin_it(*requirement_value);
           !plugin_it.IsAtEnd(); plugin_it.Advance()) {
        bool plugin_required = false;
        if (!plugin_it.value().GetAsBoolean(&plugin_required)) {
          *error = ErrorUtils::FormatErrorMessageUTF16(
              errors::kInvalidRequirement, it.key());
          return false;
        }
        if (plugin_it.key() == "npapi") {
          requirements_.npapi = plugin_required;
        } else {
          *error = ErrorUtils::FormatErrorMessageUTF16(
              errors::kInvalidRequirement, it.key());
          return false;
        }
      }
    } else if (it.key() == "3D") {
      const ListValue* features = NULL;
      if (!requirement_value->GetListWithoutPathExpansion("features",
                                                          &features) ||
          !features) {
        *error = ErrorUtils::FormatErrorMessageUTF16(
            errors::kInvalidRequirement, it.key());
        return false;
      }

      for (base::ListValue::const_iterator feature_it = features->begin();
           feature_it != features->end();
           ++feature_it) {
        std::string feature;
        if ((*feature_it)->GetAsString(&feature)) {
          if (feature == "webgl") {
            requirements_.webgl = true;
          } else if (feature == "css3d") {
            requirements_.css3d = true;
          } else {
            *error = ErrorUtils::FormatErrorMessageUTF16(
                errors::kInvalidRequirement, it.key());
            return false;
          }
        }
      }
    } else {
      *error = ASCIIToUTF16(errors::kInvalidRequirements);
      return false;
    }
  }
  return true;
}
