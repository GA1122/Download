  void VerifyTextfieldContextMenuContents(bool textfield_has_selection,
                                          bool can_undo,
                                          ui::MenuModel* menu) {
    const auto& text = textfield_->text();
    const bool is_all_selected = !text.empty() &&
        textfield_->GetSelectedRange().length() == text.length();

    int menu_index = 0;
#if defined(OS_MACOSX)
    if (textfield_has_selection) {
      EXPECT_TRUE(menu->IsEnabledAt(menu_index++  ));
      EXPECT_TRUE(menu->IsEnabledAt(menu_index++  ));
    }
#endif

    EXPECT_EQ(can_undo, menu->IsEnabledAt(menu_index++  ));
    EXPECT_TRUE(menu->IsEnabledAt(menu_index++  ));
    EXPECT_EQ(textfield_has_selection,
              menu->IsEnabledAt(menu_index++  ));
    EXPECT_EQ(textfield_has_selection,
              menu->IsEnabledAt(menu_index++  ));
    EXPECT_NE(GetClipboardText(ui::CLIPBOARD_TYPE_COPY_PASTE).empty(),
              menu->IsEnabledAt(menu_index++  ));
    EXPECT_EQ(textfield_has_selection,
              menu->IsEnabledAt(menu_index++  ));
    EXPECT_TRUE(menu->IsEnabledAt(menu_index++  ));
    EXPECT_EQ(!is_all_selected,
              menu->IsEnabledAt(menu_index++  ));
  }
