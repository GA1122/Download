ChromeExtensionsAPIClient::CreateVirtualKeyboardDelegate(
    content::BrowserContext* browser_context) const {
#if defined(OS_CHROMEOS)
  return base::MakeUnique<ChromeVirtualKeyboardDelegate>(browser_context);
#else
  return nullptr;
#endif
}
