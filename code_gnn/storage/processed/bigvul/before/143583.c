void OomInterventionTabHelper::WebContentsDestroyed() {
  StopMonitoring();
}