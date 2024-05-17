  void TestCreatePanelOnOverflow() {
    PanelManager* panel_manager = PanelManager::GetInstance();
    EXPECT_EQ(0, panel_manager->num_panels());  

    DictionaryValue empty_value;
    scoped_refptr<Extension> extension1 =
        CreateExtension(FILE_PATH_LITERAL("extension1"),
        Extension::INVALID, empty_value);
    scoped_refptr<Extension> extension2 =
        CreateExtension(FILE_PATH_LITERAL("extension2"),
        Extension::INVALID, empty_value);
    scoped_refptr<Extension> extension3 =
        CreateExtension(FILE_PATH_LITERAL("extension3"),
        Extension::INVALID, empty_value);

    Panel* panel1 = CreatePanelWithBounds(
        web_app::GenerateApplicationNameFromExtensionId(extension1->id()),
        gfx::Rect(0, 0, 250, 200));
    Panel* panel2 = CreatePanelWithBounds(
        web_app::GenerateApplicationNameFromExtensionId(extension2->id()),
        gfx::Rect(0, 0, 300, 200));
    Panel* panel3 = CreatePanelWithBounds(
        web_app::GenerateApplicationNameFromExtensionId(extension1->id()),
        gfx::Rect(0, 0, 200, 200));
    ASSERT_EQ(3, panel_manager->num_panels());

    Panel* panel4 = CreatePanelWithBounds(
        web_app::GenerateApplicationNameFromExtensionId(extension2->id()),
        gfx::Rect(0, 0, 280, 200));
    ASSERT_EQ(4, panel_manager->num_panels());
    EXPECT_LT(panel4->GetBounds().right(), panel3->GetBounds().x());
    EXPECT_GT(0, panel4->GetBounds().x());

    Panel* panel5 = CreatePanelWithBounds(
        web_app::GenerateApplicationNameFromExtensionId(extension3->id()),
        gfx::Rect(0, 0, 300, 200));
    ASSERT_EQ(5, panel_manager->num_panels());
    EXPECT_LT(panel5->GetBounds().right(), panel4->GetBounds().x());
    EXPECT_GT(0, panel5->GetBounds().x());

    CloseWindowAndWait(panel2->browser());
    ASSERT_EQ(4, panel_manager->num_panels());
    EXPECT_LT(panel4->GetBounds().right(), panel3->GetBounds().x());
    EXPECT_LE(0, panel4->GetBounds().x());
    EXPECT_GT(0, panel5->GetBounds().x());

    CloseWindowAndWait(panel3->browser());
    ASSERT_EQ(3, panel_manager->num_panels());
    EXPECT_LT(panel5->GetBounds().right(), panel4->GetBounds().x());
    EXPECT_GT(0, panel5->GetBounds().x());

    CloseWindowAndWait(panel4->browser());
    ASSERT_EQ(2, panel_manager->num_panels());
    EXPECT_LT(panel5->GetBounds().right(), panel1->GetBounds().x());
    EXPECT_LE(0, panel5->GetBounds().x());

    panel1->Close();
    panel5->Close();
  }
