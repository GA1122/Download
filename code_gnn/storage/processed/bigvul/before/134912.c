void ChromeBrowserMainPartsChromeos::PreBrowserStart() {

  external_metrics_ = new chromeos::ExternalMetrics;
  external_metrics_->Start();


  if (ui::ShouldDefaultToNaturalScroll()) {
    base::CommandLine::ForCurrentProcess()->AppendSwitch(
        chromeos::switches::kNaturalScrollDefault);
    system::InputDeviceSettings::Get()->SetTapToClick(true);
  }

  ChromeBrowserMainPartsLinux::PreBrowserStart();
}
