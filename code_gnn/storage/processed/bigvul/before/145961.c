  void RunTest(AshTestBase* test_base) {
    std::unique_ptr<aura::Window> window(
        test_base->CreateTestWindowInShellWithDelegate(
            this, 0, gfx::Rect(0, 0, 100, 100)));
    window->Show();

    GenerateEvents(window.get());

    EXPECT_EQ(2, mouse_event_count_);
    EXPECT_EQ(3, scroll_event_count_);
    EXPECT_EQ(4, touch_event_count_);
    EXPECT_EQ(10, gesture_event_count_);
    Reset();

    views::Widget* widget = views::Widget::CreateWindowWithContextAndBounds(
        new TestWindow(true), Shell::GetPrimaryRootWindow(),
        gfx::Rect(200, 200, 100, 100));
    widget->Show();
    EXPECT_TRUE(ShellPort::Get()->IsSystemModalWindowOpen());

    GenerateEvents(window.get());

    EXPECT_EQ(0, mouse_event_count_);
    EXPECT_EQ(0, scroll_event_count_);
    EXPECT_EQ(0, touch_event_count_);
    EXPECT_EQ(0, gesture_event_count_);
    Reset();

    widget->Close();
    EXPECT_FALSE(ShellPort::Get()->IsSystemModalWindowOpen());

    GenerateEvents(window.get());

    EXPECT_EQ(2, mouse_event_count_);
    EXPECT_EQ(3, scroll_event_count_);
    EXPECT_EQ(4, touch_event_count_);
    EXPECT_EQ(10, gesture_event_count_);
  }
