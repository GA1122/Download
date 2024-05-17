    void pong()
    {
        EXPECT_TRUE(isMainThread());
        webkit_support::QuitMessageLoop();
     }
