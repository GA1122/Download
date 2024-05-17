std::vector<std::pair<std::string, int> > Dispatcher::GetJsResources() {
  std::vector<std::pair<std::string, int> > resources;

  resources.push_back(std::make_pair("appView", IDR_APP_VIEW_JS));
  resources.push_back(std::make_pair("entryIdManager", IDR_ENTRY_ID_MANAGER));
  resources.push_back(std::make_pair(kEventBindings, IDR_EVENT_BINDINGS_JS));
  resources.push_back(std::make_pair("extensionOptions",
                                     IDR_EXTENSION_OPTIONS_JS));
  resources.push_back(std::make_pair("extensionOptionsAttributes",
                                     IDR_EXTENSION_OPTIONS_ATTRIBUTES_JS));
  resources.push_back(std::make_pair("extensionOptionsConstants",
                                     IDR_EXTENSION_OPTIONS_CONSTANTS_JS));
  resources.push_back(std::make_pair("extensionOptionsEvents",
                                     IDR_EXTENSION_OPTIONS_EVENTS_JS));
  resources.push_back(std::make_pair("extensionView", IDR_EXTENSION_VIEW_JS));
  resources.push_back(std::make_pair("extensionViewApiMethods",
                                     IDR_EXTENSION_VIEW_API_METHODS_JS));
  resources.push_back(std::make_pair("extensionViewAttributes",
                                     IDR_EXTENSION_VIEW_ATTRIBUTES_JS));
  resources.push_back(std::make_pair("extensionViewConstants",
                                     IDR_EXTENSION_VIEW_CONSTANTS_JS));
  resources.push_back(std::make_pair("extensionViewEvents",
                                     IDR_EXTENSION_VIEW_EVENTS_JS));
  resources.push_back(std::make_pair(
      "extensionViewInternal", IDR_EXTENSION_VIEW_INTERNAL_CUSTOM_BINDINGS_JS));
  resources.push_back(std::make_pair("guestView", IDR_GUEST_VIEW_JS));
  resources.push_back(std::make_pair("guestViewAttributes",
                                     IDR_GUEST_VIEW_ATTRIBUTES_JS));
  resources.push_back(std::make_pair("guestViewContainer",
                                     IDR_GUEST_VIEW_CONTAINER_JS));
  resources.push_back(std::make_pair("guestViewDeny", IDR_GUEST_VIEW_DENY_JS));
  resources.push_back(std::make_pair("guestViewEvents",
                                     IDR_GUEST_VIEW_EVENTS_JS));

  if (content::BrowserPluginGuestMode::UseCrossProcessFramesForGuests()) {
    resources.push_back(std::make_pair("guestViewIframe",
                                       IDR_GUEST_VIEW_IFRAME_JS));
    resources.push_back(std::make_pair("guestViewIframeContainer",
                                       IDR_GUEST_VIEW_IFRAME_CONTAINER_JS));
  }

  resources.push_back(std::make_pair("imageUtil", IDR_IMAGE_UTIL_JS));
  resources.push_back(std::make_pair("json_schema", IDR_JSON_SCHEMA_JS));
  resources.push_back(std::make_pair("lastError", IDR_LAST_ERROR_JS));
  resources.push_back(std::make_pair("messaging", IDR_MESSAGING_JS));
  resources.push_back(std::make_pair("messaging_utils",
                                     IDR_MESSAGING_UTILS_JS));
  resources.push_back(std::make_pair(kSchemaUtils, IDR_SCHEMA_UTILS_JS));
  resources.push_back(std::make_pair("sendRequest", IDR_SEND_REQUEST_JS));
  resources.push_back(std::make_pair("setIcon", IDR_SET_ICON_JS));
  resources.push_back(std::make_pair("test", IDR_TEST_CUSTOM_BINDINGS_JS));
  resources.push_back(
      std::make_pair("test_environment_specific_bindings",
                     IDR_BROWSER_TEST_ENVIRONMENT_SPECIFIC_BINDINGS_JS));
  resources.push_back(std::make_pair("uncaught_exception_handler",
                                     IDR_UNCAUGHT_EXCEPTION_HANDLER_JS));
  resources.push_back(std::make_pair("utils", IDR_UTILS_JS));
  resources.push_back(std::make_pair("webRequest",
                                     IDR_WEB_REQUEST_CUSTOM_BINDINGS_JS));
  resources.push_back(
       std::make_pair("webRequestInternal",
                      IDR_WEB_REQUEST_INTERNAL_CUSTOM_BINDINGS_JS));
  resources.push_back(std::make_pair("webView", IDR_WEB_VIEW_JS));
  resources.push_back(std::make_pair("webViewActionRequests",
                                     IDR_WEB_VIEW_ACTION_REQUESTS_JS));
  resources.push_back(std::make_pair("webViewApiMethods",
                                     IDR_WEB_VIEW_API_METHODS_JS));
  resources.push_back(std::make_pair("webViewAttributes",
                                     IDR_WEB_VIEW_ATTRIBUTES_JS));
  resources.push_back(std::make_pair("webViewConstants",
                                     IDR_WEB_VIEW_CONSTANTS_JS));
  resources.push_back(std::make_pair("webViewEvents", IDR_WEB_VIEW_EVENTS_JS));
  resources.push_back(std::make_pair("webViewInternal",
                                     IDR_WEB_VIEW_INTERNAL_CUSTOM_BINDINGS_JS));
  resources.push_back(
      std::make_pair("webViewExperimental", IDR_WEB_VIEW_EXPERIMENTAL_JS));
  resources.push_back(
      std::make_pair(mojo::kBindingsModuleName, IDR_MOJO_BINDINGS_JS));
  resources.push_back(
      std::make_pair(mojo::kBufferModuleName, IDR_MOJO_BUFFER_JS));
  resources.push_back(
      std::make_pair(mojo::kCodecModuleName, IDR_MOJO_CODEC_JS));
  resources.push_back(
      std::make_pair(mojo::kConnectionModuleName, IDR_MOJO_CONNECTION_JS));
  resources.push_back(
      std::make_pair(mojo::kConnectorModuleName, IDR_MOJO_CONNECTOR_JS));
  resources.push_back(
      std::make_pair(mojo::kRouterModuleName, IDR_MOJO_ROUTER_JS));
  resources.push_back(
      std::make_pair(mojo::kUnicodeModuleName, IDR_MOJO_UNICODE_JS));
  resources.push_back(
      std::make_pair(mojo::kValidatorModuleName, IDR_MOJO_VALIDATOR_JS));
  resources.push_back(std::make_pair("async_waiter", IDR_ASYNC_WAITER_JS));
  resources.push_back(std::make_pair("data_receiver", IDR_DATA_RECEIVER_JS));
  resources.push_back(std::make_pair("data_sender", IDR_DATA_SENDER_JS));
  resources.push_back(std::make_pair("keep_alive", IDR_KEEP_ALIVE_JS));
  resources.push_back(std::make_pair("extensions/common/mojo/keep_alive.mojom",
                                     IDR_KEEP_ALIVE_MOJOM_JS));
  resources.push_back(std::make_pair("device/serial/data_stream.mojom",
                                     IDR_DATA_STREAM_MOJOM_JS));
  resources.push_back(
      std::make_pair("device/serial/data_stream_serialization.mojom",
                     IDR_DATA_STREAM_SERIALIZATION_MOJOM_JS));
  resources.push_back(std::make_pair("stash_client", IDR_STASH_CLIENT_JS));
  resources.push_back(
      std::make_pair("extensions/common/mojo/stash.mojom", IDR_STASH_MOJOM_JS));

  resources.push_back(
      std::make_pair("app.runtime", IDR_APP_RUNTIME_CUSTOM_BINDINGS_JS));
  resources.push_back(
      std::make_pair("app.window", IDR_APP_WINDOW_CUSTOM_BINDINGS_JS));
  resources.push_back(
      std::make_pair("declarativeWebRequest",
                     IDR_DECLARATIVE_WEBREQUEST_CUSTOM_BINDINGS_JS));
  resources.push_back(
      std::make_pair("displaySource",
                     IDR_DISPLAY_SOURCE_CUSTOM_BINDINGS_JS));
  resources.push_back(
      std::make_pair("contextMenus", IDR_CONTEXT_MENUS_CUSTOM_BINDINGS_JS));
  resources.push_back(
      std::make_pair("contextMenusHandlers", IDR_CONTEXT_MENUS_HANDLERS_JS));
  resources.push_back(
      std::make_pair("extension", IDR_EXTENSION_CUSTOM_BINDINGS_JS));
  resources.push_back(std::make_pair("i18n", IDR_I18N_CUSTOM_BINDINGS_JS));
  resources.push_back(std::make_pair(
      "mimeHandlerPrivate", IDR_MIME_HANDLER_PRIVATE_CUSTOM_BINDINGS_JS));
  resources.push_back(std::make_pair("extensions/common/api/mime_handler.mojom",
                                     IDR_MIME_HANDLER_MOJOM_JS));
  resources.push_back(
      std::make_pair("mojoPrivate", IDR_MOJO_PRIVATE_CUSTOM_BINDINGS_JS));
  resources.push_back(
      std::make_pair("permissions", IDR_PERMISSIONS_CUSTOM_BINDINGS_JS));
  resources.push_back(std::make_pair("printerProvider",
                                     IDR_PRINTER_PROVIDER_CUSTOM_BINDINGS_JS));
  resources.push_back(
      std::make_pair("runtime", IDR_RUNTIME_CUSTOM_BINDINGS_JS));
  resources.push_back(std::make_pair("windowControls", IDR_WINDOW_CONTROLS_JS));
  resources.push_back(
      std::make_pair("webViewRequest",
                     IDR_WEB_VIEW_REQUEST_CUSTOM_BINDINGS_JS));
  resources.push_back(std::make_pair("binding", IDR_BINDING_JS));

  if (base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kEnableMojoSerialService)) {
    resources.push_back(
        std::make_pair("serial", IDR_SERIAL_CUSTOM_BINDINGS_JS));
  }
  resources.push_back(std::make_pair("serial_service", IDR_SERIAL_SERVICE_JS));
  resources.push_back(
      std::make_pair("device/serial/serial.mojom", IDR_SERIAL_MOJOM_JS));
  resources.push_back(std::make_pair("device/serial/serial_serialization.mojom",
                                     IDR_SERIAL_SERIALIZATION_MOJOM_JS));

  resources.push_back(std::make_pair("StorageArea", IDR_STORAGE_AREA_JS));

  resources.push_back(std::make_pair("platformApp", IDR_PLATFORM_APP_JS));

#if defined(ENABLE_MEDIA_ROUTER)
  resources.push_back(
      std::make_pair("chrome/browser/media/router/mojo/media_router.mojom",
                     IDR_MEDIA_ROUTER_MOJOM_JS));
  resources.push_back(
      std::make_pair("media_router_bindings", IDR_MEDIA_ROUTER_BINDINGS_JS));
#endif   

  return resources;
}
