  void TestDragging(int delta_x,
                    int delta_y,
                    size_t drag_index,
                    std::vector<int> expected_delta_x_after_drag,
                    std::vector<int> expected_delta_x_after_finish,
                    std::vector<gfx::Rect> expected_bounds_after_cancel,
                    int drag_action) {
    std::vector<Panel*> panels = PanelManager::GetInstance()->panels();

    std::vector<gfx::Rect> test_begin_bounds = GetAllPanelBounds();

    NativePanel* panel_to_drag = panels[drag_index]->native_panel();
    scoped_ptr<NativePanelTesting> panel_testing_to_drag(
        NativePanelTesting::Create(panel_to_drag));

    if (drag_action & DRAG_ACTION_BEGIN) {
      panel_testing_to_drag->PressLeftMouseButtonTitlebar(
          panels[drag_index]->GetBounds().origin());
      EXPECT_EQ(test_begin_bounds, GetPanelBounds(panels));
    }

    panel_testing_to_drag->DragTitlebar(delta_x, delta_y);

    EXPECT_EQ(AddXDeltaToBounds(test_begin_bounds, expected_delta_x_after_drag),
              GetPanelBounds(panels));

    if (drag_action & DRAG_ACTION_CANCEL) {
      panel_testing_to_drag->CancelDragTitlebar();
      EXPECT_EQ(expected_bounds_after_cancel, GetAllPanelBounds());
    } else if (drag_action & DRAG_ACTION_FINISH) {
      panel_testing_to_drag->FinishDragTitlebar();
      EXPECT_EQ(
          AddXDeltaToBounds(test_begin_bounds, expected_delta_x_after_finish),
          GetPanelBounds(panels));
    }
  }
