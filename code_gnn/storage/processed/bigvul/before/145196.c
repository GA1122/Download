void ChromeExtensionsDispatcherDelegate::SetChannel(int channel) {
  extensions::SetCurrentChannel(static_cast<version_info::Channel>(channel));
  if (extensions::feature_util::ExtensionServiceWorkersEnabled()) {
    blink::WebSecurityPolicy::registerURLSchemeAsAllowingServiceWorkers(
        blink::WebString::fromUTF8(extensions::kExtensionScheme));
  }
}
