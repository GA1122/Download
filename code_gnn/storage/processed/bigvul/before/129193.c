  FramebufferManagerTest()
      : manager_(1, 1),
        texture_manager_(NULL,
                         new FeatureInfo(),
                         kMaxTextureSize,
                         kMaxCubemapSize,
                         kUseDefaultTextures),
        renderbuffer_manager_(NULL,
                              kMaxRenderbufferSize,
                              kMaxSamples,
                              kDepth24Supported) {}
