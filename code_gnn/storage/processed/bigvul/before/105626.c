 bool SetMongooseOptions(struct mg_context* ctx,
                        const std::string& port,
                        const std::string& root) {
  if (!mg_set_option(ctx, "ports", port.c_str())) {
    std::cout << "ChromeDriver cannot bind to port ("
              << port.c_str() << ")" << std::endl;
    return false;
  }
  if (root.length())
    mg_set_option(ctx, "root", root.c_str());
  mg_set_option(ctx, "idle_time", "1");
  return true;
}
