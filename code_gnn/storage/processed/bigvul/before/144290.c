WizardController* LoginDisplayHostWebUI::CreateWizardController() {
  OobeUI* oobe_ui = GetOobeUI();
  return new WizardController(this, oobe_ui);
}
