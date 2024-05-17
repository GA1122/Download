  void SetupLeftRightBitmap(const gfx::Size& copy_size, SkBitmap* bitmap) {
    bitmap->setConfig(
        SkBitmap::kARGB_8888_Config, copy_size.width(), copy_size.height());
    bitmap->allocPixels();
    bitmap->eraseARGB(255, 0, 255, 255);
    {
      SkAutoLockPixels lock(*bitmap);
      for (int i = 0; i < copy_size.width() / 2; ++i) {
        for (int j = 0; j < copy_size.height(); ++j) {
          *(bitmap->getAddr32(copy_size.width() / 2 + i, j)) =
              SkColorSetARGB(255, 255, 255, 0);
        }
      }
    }
  }
