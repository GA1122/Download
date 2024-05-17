MATCHER_P(Message, type, "") {
  return arg.type() == static_cast<uint32>(type);
}
