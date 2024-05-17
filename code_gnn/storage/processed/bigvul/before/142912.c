EnumerationHistogram& HTMLMediaElement::ShowControlsHistogram() const {
  if (IsHTMLVideoElement()) {
    DEFINE_STATIC_LOCAL(EnumerationHistogram, histogram,
                        ("Media.Controls.Show.Video", kMediaControlsShowMax));
    return histogram;
  }

  DEFINE_STATIC_LOCAL(EnumerationHistogram, histogram,
                      ("Media.Controls.Show.Audio", kMediaControlsShowMax));
  return histogram;
}
