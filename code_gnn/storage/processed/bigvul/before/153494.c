bool TabStrip::TitlebarBackgroundIsTransparent() const {
#if defined(OS_WIN)
  if (base::win::GetVersion() >= base::win::Version::WIN8)
    return false;
#endif
  return GetWidget()->ShouldWindowContentsBeTransparent();
}
