Error* Automation::CheckAlertsSupported() {
  return CheckVersion(
      768, 0, "Alerts are not supported for this version of Chrome");
}
