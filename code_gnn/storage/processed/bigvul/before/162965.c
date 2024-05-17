static const std::string& GetMessagePrefix() {
  CR_DEFINE_STATIC_LOCAL(std::string, message_prefix, ("Manifest: "));
  return message_prefix;
}
