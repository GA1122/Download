bool IsViewsBrowserCocoa() {
  return !base::FeatureList::IsEnabled(kViewsBrowserWindows) &&
      !base::FeatureList::IsEnabled(kExperimentalUi);
}
