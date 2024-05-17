AppWindowCustomBindings::AppWindowCustomBindings(
    ExtensionDispatcher* extension_dispatcher)
    : ChromeV8Extension(extension_dispatcher) {
  RouteStaticFunction("GetView", &GetView);
}
