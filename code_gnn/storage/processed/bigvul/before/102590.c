void EnterpriseEnrollmentScreen::OnConfirmationClosed() {
  auth_fetcher_.reset();
  get_screen_observer()->OnExit(
      ScreenObserver::ENTERPRISE_ENROLLMENT_COMPLETED);
}
