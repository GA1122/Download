bool AutofillManager::ShouldParseForms(const std::vector<FormData>& forms,
                                       const base::TimeTicks timestamp) {
  bool enabled = IsAutofillEnabled();
  if (!has_logged_autofill_enabled_) {
    AutofillMetrics::LogIsAutofillEnabledAtPageLoad(enabled);
    has_logged_autofill_enabled_ = true;
  }

  return enabled;
}
