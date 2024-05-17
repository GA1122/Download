  FrameFetchContextModifyRequestTest()
      : example_origin(SecurityOrigin::Create(
            KURL(kParsedURLString, "https://example.test/"))),
        secure_origin(SecurityOrigin::Create(
            KURL(kParsedURLString, "https://secureorigin.test/image.png"))) {}
