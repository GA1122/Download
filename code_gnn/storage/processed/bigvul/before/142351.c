 void PasswordAccessoryControllerImpl::SavePasswordsForOrigin(
    const std::map<base::string16, const PasswordForm*>& best_matches,
    const url::Origin& origin) {
  std::vector<SuggestionElementData>* suggestions =
      &origin_suggestions_[origin];
  suggestions->clear();
  for (const auto& pair : best_matches) {
    const PasswordForm* form = pair.second;
    suggestions->emplace_back(form->password_value, GetDisplayUsername(*form),
                               !form->username_value.empty());
   }
 }
