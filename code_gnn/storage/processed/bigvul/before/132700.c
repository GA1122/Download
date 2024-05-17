void RegisterSideloadedTypefaces(SkFontMgr* fontmgr) {
  std::vector<std::string> files = GetSideloadFontFiles();
  for (std::vector<std::string>::const_iterator i(files.begin());
       i != files.end();
       ++i) {
    SkTypeface* typeface = fontmgr->createFromFile(i->c_str());
    DoPreSandboxWarmupForTypeface(typeface);
    blink::WebFontRendering::addSideloadedFontForTesting(typeface);
  }
}
