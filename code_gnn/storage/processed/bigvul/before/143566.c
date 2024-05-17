bool OomInterventionTabHelper::IsEnabled() {
  return OomInterventionConfig::GetInstance()->is_intervention_enabled();
}
