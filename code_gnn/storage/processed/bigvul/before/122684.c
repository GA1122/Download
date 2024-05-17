bool Extension::LoadIncognitoMode(string16* error) {
  incognito_split_mode_ = is_app();
  if (!manifest_->HasKey(keys::kIncognito))
    return true;
  std::string value;
  if (!manifest_->GetString(keys::kIncognito, &value)) {
    *error = ASCIIToUTF16(errors::kInvalidIncognitoBehavior);
    return false;
  }
  if (value == values::kIncognitoSpanning) {
    incognito_split_mode_ = false;
  } else if (value == values::kIncognitoSplit) {
    incognito_split_mode_ = true;
  } else {
    *error = ASCIIToUTF16(errors::kInvalidIncognitoBehavior);
    return false;
  }
  return true;
}
