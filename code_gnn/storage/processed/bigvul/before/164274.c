  ActiveTabTest()
      : current_channel(version_info::Channel::DEV),
        extension(CreateTestExtension("deadbeef", true, false)),
        another_extension(CreateTestExtension("feedbeef", true, false)),
        extension_without_active_tab(CreateTestExtension("badbeef",
                                                         false,
                                                         false)),
        extension_with_tab_capture(CreateTestExtension("cafebeef",
                                                       true,
                                                       true)) {}
