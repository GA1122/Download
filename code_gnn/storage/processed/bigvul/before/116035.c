void ExtensionInstalledBubble::Show(const Extension* extension,
                                    Browser *browser,
                                    const SkBitmap& icon) {
  new ExtensionInstalledBubble(extension, browser, icon);
}
