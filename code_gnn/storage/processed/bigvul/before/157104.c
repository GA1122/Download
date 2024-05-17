  int64_t preload_high() {
    CHECK(loader());
    return loader()->preload_high();
  }
