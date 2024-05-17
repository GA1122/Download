  int64_t preload_low() {
    CHECK(loader());
    return loader()->preload_low();
  }
