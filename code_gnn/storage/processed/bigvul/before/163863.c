bool WaitForTabsAndPopups(Browser* browser,
                          int num_tabs,
                          int num_popups) {
  SCOPED_TRACE(
      base::StringPrintf("WaitForTabsAndPopups tabs:%d, popups:%d",
                         num_tabs, num_popups));
  ++num_tabs;
  size_t num_browsers = static_cast<size_t>(num_popups) + 1;

  const base::TimeDelta kWaitTime = base::TimeDelta::FromSeconds(10);
  base::TimeTicks end_time = base::TimeTicks::Now() + kWaitTime;
  while (base::TimeTicks::Now() < end_time) {
    if (chrome::GetBrowserCount(browser->profile()) == num_browsers &&
        browser->tab_strip_model()->count() == num_tabs)
      break;

    content::RunAllPendingInMessageLoop();
  }

  EXPECT_EQ(num_browsers, chrome::GetBrowserCount(browser->profile()));
  EXPECT_EQ(num_tabs, browser->tab_strip_model()->count());

  int num_popups_seen = 0;
  for (auto* b : *BrowserList::GetInstance()) {
    if (b == browser)
      continue;

    EXPECT_TRUE(b->is_type_popup());
    ++num_popups_seen;
  }
  EXPECT_EQ(num_popups, num_popups_seen);

  return ((num_browsers == chrome::GetBrowserCount(browser->profile())) &&
          (num_tabs == browser->tab_strip_model()->count()) &&
          (num_popups == num_popups_seen));
}
