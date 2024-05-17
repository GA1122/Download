bool IsPortValid(int port) {
  return port >= 0 && port <= std::numeric_limits<uint16_t>::max();
}
