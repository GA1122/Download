void RenderWidgetHostViewAura::OnUpdateTextInputStateCalled(
    TextInputManager* text_input_manager,
    RenderWidgetHostViewBase* updated_view,
    bool did_update_state) {
  DCHECK_EQ(text_input_manager_, text_input_manager);

  if (!GetInputMethod())
    return;

  if (did_update_state)
    GetInputMethod()->OnTextInputTypeChanged(this);

  const TextInputState* state = text_input_manager_->GetTextInputState();
  if (state && state->show_ime_if_needed &&
      GetInputMethod()->GetTextInputClient() == this) {
    GetInputMethod()->ShowImeIfNeeded();
  }

  if (auto* render_widget_host =
          RenderWidgetHostImpl::From(updated_view->GetRenderWidgetHost())) {
    render_widget_host->RequestCompositionUpdates(
        false  ,
        state &&
            (state->type != ui::TEXT_INPUT_TYPE_NONE)  );
  }
}
