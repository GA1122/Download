void DevToolsWindow::DoAction(const DevToolsToggleAction& action) {
  switch (action.type()) {
    case DevToolsToggleAction::kInspect:
      bindings_->CallClientFunction("DevToolsAPI.enterInspectElementMode", NULL,
                                    NULL, NULL);
      break;

    case DevToolsToggleAction::kShowElementsPanel:
    case DevToolsToggleAction::kShowConsolePanel:
    case DevToolsToggleAction::kShow:
    case DevToolsToggleAction::kToggle:
      break;

    case DevToolsToggleAction::kReveal: {
      const DevToolsToggleAction::RevealParams* params =
          action.params();
      CHECK(params);
      base::Value url_value(params->url);
      base::Value line_value(static_cast<int>(params->line_number));
      base::Value column_value(static_cast<int>(params->column_number));
      bindings_->CallClientFunction("DevToolsAPI.revealSourceLine",
                                    &url_value, &line_value, &column_value);
      break;
    }
    default:
      NOTREACHED();
      break;
  }
}
