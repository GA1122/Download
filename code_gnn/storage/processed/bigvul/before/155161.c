void OmniboxViewViews::SelectAllForUserGesture() {
  if (base::FeatureList::IsEnabled(omnibox::kOneClickUnelide) &&
      UnapplySteadyStateElisions(UnelisionGesture::OTHER)) {
    TextChanged();
  }

  SelectAll(true);
}
