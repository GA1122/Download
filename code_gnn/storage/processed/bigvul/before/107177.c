void AutoFillManager::ImportFormData(const FormStructure& submitted_form) {
  std::vector<const FormStructure*> import;
  import.push_back(&submitted_form);
  if (!personal_data_->ImportFormData(import))
    return;

  AutoFillProfile* profile;
  CreditCard* credit_card;
  personal_data_->GetImportedFormData(&profile, &credit_card);

  if (credit_card && tab_contents_) {
    tab_contents_->AddInfoBar(new AutoFillCCInfoBarDelegate(tab_contents_,
                                                            this));
  }
}
