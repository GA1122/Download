std::string GetRestrictParameterPref(PrefService* policy_pref_service) {
  std::string parameter;
#if defined(OS_CHROMEOS)
  chromeos::CrosSettings::Get()->GetString(
      chromeos::kVariationsRestrictParameter, &parameter);
#else
  if (policy_pref_service) {
    parameter =
        policy_pref_service->GetString(prefs::kVariationsRestrictParameter);
  }
#endif
  return parameter;
}
