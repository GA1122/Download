  explicit ProtocolHandlerThrottle(
      const scoped_refptr<ProtocolHandlerRegistry::IOThreadDelegate>&
          protocol_handler_registry)
      : protocol_handler_registry_(protocol_handler_registry) {}
