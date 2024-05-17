void ChromeBrowserMainPartsChromeos::PostBrowserStart() {
  if (system::InputDeviceSettings::Get()->ForceKeyboardDrivenUINavigation()) {
    content::ServiceManagerConnection* connection =
        content::ServiceManagerConnection::GetForProcess();
    ash::mojom::EventRewriterControllerPtr event_rewriter_controller_ptr;
    connection->GetConnector()->BindInterface(ash::mojom::kServiceName,
                                              &event_rewriter_controller_ptr);
    event_rewriter_controller_ptr->SetKeyboardDrivenEventRewriterEnabled(true);
  }

  spoken_feedback_event_rewriter_delegate_ =
      std::make_unique<SpokenFeedbackEventRewriterDelegate>();

  if (chromeos::GetAshConfig() != ash::Config::MASH) {
    ash::EventRewriterController* event_rewriter_controller =
        ash::Shell::Get()->event_rewriter_controller();
    event_rewriter_delegate_ = std::make_unique<EventRewriterDelegateImpl>();
    event_rewriter_controller->AddEventRewriter(
        std::make_unique<ui::EventRewriterChromeOS>(
            event_rewriter_delegate_.get(),
            ash::Shell::Get()->sticky_keys_controller()));
  }

  shutdown_policy_forwarder_ = std::make_unique<ShutdownPolicyForwarder>();

  if (base::FeatureList::IsEnabled(
          features::kAdaptiveScreenBrightnessLogging)) {
    adaptive_screen_brightness_manager_ =
        power::ml::AdaptiveScreenBrightnessManager::CreateInstance();
  }

  if (base::FeatureList::IsEnabled(features::kUserActivityEventLogging)) {
    user_activity_controller_ =
        std::make_unique<power::ml::UserActivityController>();
  }

  ChromeBrowserMainPartsLinux::PostBrowserStart();
}
