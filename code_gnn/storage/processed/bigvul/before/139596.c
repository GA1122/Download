void MimeUtil::InitializeMimeTypeMaps() {
#if BUILDFLAG(USE_PROPRIETARY_CODECS)
  allow_proprietary_codecs_ = true;
#endif

  AddSupportedMediaFormats();
}
