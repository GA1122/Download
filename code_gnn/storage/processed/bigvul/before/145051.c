BlinkMediaTestSuite::BlinkMediaTestSuite(int argc, char** argv)
    : TestSuite(argc, argv),
      blink_platform_support_(new TestBlinkPlatformSupport()) {
}
