  static void CheckForExpectedLayoutAndVisibilityOfElements(const Tab& tab) {
    if (tab.data_.pinned) {
      EXPECT_EQ(1, VisibleIconCount(tab));
      if (tab.data_.alert_state != TabAlertState::NONE) {
        EXPECT_FALSE(tab.showing_icon_);
        EXPECT_TRUE(tab.showing_alert_indicator_);
      } else {
        EXPECT_TRUE(tab.showing_icon_);
        EXPECT_FALSE(tab.showing_alert_indicator_);
      }
      EXPECT_FALSE(tab.title_->GetVisible());
      EXPECT_FALSE(tab.showing_close_button_);
    } else if (tab.IsActive()) {
      EXPECT_TRUE(tab.showing_close_button_);
      switch (VisibleIconCount(tab)) {
        case 1:
          EXPECT_FALSE(tab.showing_icon_);
          EXPECT_FALSE(tab.showing_alert_indicator_);
          break;
        case 2:
          if (tab.data_.alert_state != TabAlertState::NONE) {
            EXPECT_FALSE(tab.showing_icon_);
            EXPECT_TRUE(tab.showing_alert_indicator_);
          } else {
            EXPECT_TRUE(tab.showing_icon_);
            EXPECT_FALSE(tab.showing_alert_indicator_);
          }
          break;
        default:
          EXPECT_EQ(3, VisibleIconCount(tab));
          EXPECT_TRUE(tab.data_.alert_state != TabAlertState::NONE);
          break;
      }
    } else {   
      switch (VisibleIconCount(tab)) {
        case 1:
          EXPECT_FALSE(tab.showing_close_button_);
          if (tab.data_.alert_state == TabAlertState::NONE) {
            EXPECT_FALSE(tab.showing_alert_indicator_);
            EXPECT_TRUE(tab.showing_icon_);
          } else {
            EXPECT_FALSE(tab.showing_icon_);
            EXPECT_TRUE(tab.showing_alert_indicator_);
          }
          break;
        case 2:
          EXPECT_TRUE(tab.showing_icon_);
          if (tab.data_.alert_state != TabAlertState::NONE)
            EXPECT_TRUE(tab.showing_alert_indicator_);
          else
            EXPECT_FALSE(tab.showing_alert_indicator_);
          break;
        default:
          EXPECT_EQ(3, VisibleIconCount(tab));
          EXPECT_TRUE(tab.data_.alert_state != TabAlertState::NONE);
      }
    }

    const gfx::Rect contents_bounds = tab.GetContentsBounds();
    if (tab.showing_icon_) {
      if (tab.center_icon_) {
        EXPECT_LE(tab.icon_->x(), contents_bounds.x());
      } else {
        EXPECT_LE(contents_bounds.x(), tab.icon_->x());
      }
      if (tab.title_->GetVisible())
        EXPECT_LE(tab.icon_->bounds().right(), tab.title_->x());
      EXPECT_LE(contents_bounds.y(), tab.icon_->y());
      EXPECT_LE(tab.icon_->bounds().bottom(), contents_bounds.bottom());
    }

    if (tab.showing_icon_ && tab.showing_alert_indicator_) {
      int icon_visual_right = tab.icon_->bounds().x() + gfx::kFaviconSize;
      EXPECT_LE(icon_visual_right, GetAlertIndicatorBounds(tab).x());
    }

    if (tab.showing_alert_indicator_) {
      if (tab.title_->GetVisible()) {
        EXPECT_LE(tab.title_->bounds().right(),
                  GetAlertIndicatorBounds(tab).x());
      }
      if (tab.center_icon_) {
        EXPECT_LE(contents_bounds.right(),
                  GetAlertIndicatorBounds(tab).right());
      } else {
        EXPECT_LE(GetAlertIndicatorBounds(tab).right(),
                  contents_bounds.right());
      }
      EXPECT_LE(contents_bounds.y(), GetAlertIndicatorBounds(tab).y());
      EXPECT_LE(GetAlertIndicatorBounds(tab).bottom(),
                contents_bounds.bottom());
    }
    if (tab.showing_alert_indicator_ && tab.showing_close_button_) {
      EXPECT_LE(GetAlertIndicatorBounds(tab).right(),
                tab.close_button_->bounds().x() +
                    tab.close_button_->GetInsets().left());
    }
    if (tab.showing_close_button_) {
      if (tab.title_->GetVisible()) {
        EXPECT_LE(tab.title_->bounds().right(),
                  tab.close_button_->bounds().x() +
                      tab.close_button_->GetInsets().left());
      }
      const gfx::Rect close_bounds = tab.close_button_->GetContentsBounds();
      EXPECT_LE(close_bounds.right(), tab.GetLocalBounds().right());
      EXPECT_LE(contents_bounds.y(), close_bounds.y());
      EXPECT_LE(close_bounds.bottom(), contents_bounds.bottom());
    }
  }
