void ChromeContentBrowserClientExtensionsPart::ResourceDispatcherHostCreated() {
  content::ResourceDispatcherHost::Get()->RegisterInterceptor(
      "Origin", kExtensionScheme, base::Bind(&OnHttpHeaderReceived));
}
