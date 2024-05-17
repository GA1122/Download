void ManualFillingControllerImpl::ShowWhenKeyboardIsVisible(
    FillingSource source) {
  if (source == FillingSource::AUTOFILL &&
      !base::FeatureList::IsEnabled(
          autofill::features::kAutofillKeyboardAccessory)) {
    return;
  }
  visible_sources_.insert(source);
  view_->ShowWhenKeyboardIsVisible();
}
