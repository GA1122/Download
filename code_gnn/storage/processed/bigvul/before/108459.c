 void ChromeRenderProcessObserver::OnSetFieldTrialGroup(
     const std::string& field_trial_name,
     const std::string& group_name) {
  base::FieldTrialList::CreateFieldTrial(field_trial_name, group_name);
  chrome_variations::SetChildProcessLoggingVariationList();
}
