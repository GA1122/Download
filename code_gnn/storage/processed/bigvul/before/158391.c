  void ActivateViewForTextInputManager(RenderWidgetHostViewBase* view,
                                       ui::TextInputType type) {
    DCHECK_NE(ui::TEXT_INPUT_TYPE_NONE, type);
    if (render_widget_host_delegate()->GetFocusedRenderWidgetHost(
            widget_host_) != view->GetRenderWidgetHost()) {
      render_widget_host_delegate()->set_focused_widget(view->host());
    }

    TextInputManager* manager =
        static_cast<RenderWidgetHostImpl*>(view->GetRenderWidgetHost())
            ->delegate()
            ->GetTextInputManager();
    if (manager->GetActiveWidget()) {
      manager->active_view_for_testing()->TextInputStateChanged(
          TextInputState());
    }

    if (!view)
      return;

    TextInputState state_with_type_text;
    state_with_type_text.type = type;
    view->TextInputStateChanged(state_with_type_text);
  }
