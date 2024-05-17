  explicit ExtensionImpl(ExtensionDispatcher* extension_dispatcher)
    : ExtensionBase(kExtensionName,
                    GetStringResource(IDR_EXTENSION_PROCESS_BINDINGS_JS),
                    arraysize(kExtensionDeps),
                    kExtensionDeps,
                    extension_dispatcher) {
  }
