void ShowExtensionInstalledBubble(
    const Extension* extension,
    Browser* browser,
    const SkBitmap& icon,
    Profile* profile) {
  ExtensionInstalledBubbleGtk::Show(extension, browser, icon);
}
