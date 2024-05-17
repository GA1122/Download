void DevToolsWindow::DoAction(const DevToolsToggleAction& action) {
  switch (action.type()) {
    case DevToolsToggleAction::kShowConsole: {
      base::StringValue panel_name("console");
      bindings_->CallClientFunction("DevToolsAPI.showPanel", &panel_name, NULL,
                                    NULL);
      break;
    }
    case DevToolsToggleAction::kShowSecurityPanel: {
      base::StringValue panel_name("security");
      bindings_->CallClientFunction("DevToolsAPI.showPanel", &panel_name, NULL,
                                    NULL);
      break;
    }
    case DevToolsToggleAction::kInspect:
      bindings_->CallClientFunction(
          "DevToolsAPI.enterInspectElementMode", NULL, NULL, NULL);
      break;

    case DevToolsToggleAction::kShow:
    case DevToolsToggleAction::kToggle:
      break;

    case DevToolsToggleAction::kReveal: {
      const DevToolsToggleAction::RevealParams* params =
          action.params();
      CHECK(params);
      base::StringValue url_value(params->url);
      base::FundamentalValue line_value(static_cast<int>(params->line_number));
      base::FundamentalValue column_value(
          static_cast<int>(params->column_number));
      bindings_->CallClientFunction("DevToolsAPI.revealSourceLine",
                                    &url_value, &line_value, &column_value);
      break;
    }
    default:
      NOTREACHED();
      break;
  }
}
