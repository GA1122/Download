  FakeExternalProtocolHandlerDelegate()
      : block_state_(ExternalProtocolHandler::BLOCK),
        os_state_(shell_integration::UNKNOWN_DEFAULT),
        has_launched_(false),
        has_prompted_(false),
        has_blocked_(false) {}
