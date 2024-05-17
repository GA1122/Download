static void CheckCcScrollNode(const ScrollPaintPropertyNode& blink_scroll,
                              const cc::ScrollNode& cc_scroll) {
  EXPECT_EQ(static_cast<gfx::Size>(blink_scroll.ContainerRect().Size()),
            cc_scroll.container_bounds);
  EXPECT_EQ(static_cast<gfx::Size>(blink_scroll.ContentsRect().Size()),
            cc_scroll.bounds);
  EXPECT_EQ(blink_scroll.UserScrollableHorizontal(),
            cc_scroll.user_scrollable_horizontal);
  EXPECT_EQ(blink_scroll.UserScrollableVertical(),
            cc_scroll.user_scrollable_vertical);
  EXPECT_EQ(blink_scroll.GetCompositorElementId(), cc_scroll.element_id);
  EXPECT_EQ(blink_scroll.GetMainThreadScrollingReasons(),
            cc_scroll.main_thread_scrolling_reasons);
}
