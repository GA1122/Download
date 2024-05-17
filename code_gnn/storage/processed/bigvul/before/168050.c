bool AutofillManager::GetProfile(int unique_id,
                                 const AutofillProfile** profile) {
  std::string credit_card_id;
  std::string profile_id;
  SplitFrontendID(unique_id, &credit_card_id, &profile_id);
  *profile = nullptr;
  if (base::IsValidGUID(profile_id))
    *profile = personal_data_->GetProfileByGUID(profile_id);
  return !!*profile;
}
