void ShowExtensionInstalledBubble(
    const Extension* extension,
    Browser* browser,
    const SkBitmap& icon,
    Profile* profile) {
  ExtensionInstalledBubble::Show(extension, browser, icon);
}
