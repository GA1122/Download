GLES2Implementation::DeferredErrorCallback::DeferredErrorCallback(
    std::string message,
    int32_t id)
    : message(std::move(message)), id(id) {}
