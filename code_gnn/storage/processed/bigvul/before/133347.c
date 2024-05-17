AshWindowTreeHost* WindowTreeHostManager::AddWindowTreeHostForDisplay(
    const display::Display& display,
    const AshWindowTreeHostInitParams& init_params) {
  static int host_count = 0;
  const DisplayInfo& display_info =
      GetDisplayManager()->GetDisplayInfo(display.id());
  AshWindowTreeHostInitParams params_with_bounds(init_params);
  params_with_bounds.initial_bounds = display_info.bounds_in_native();
  params_with_bounds.offscreen =
      display.id() == DisplayManager::kUnifiedDisplayId;
  AshWindowTreeHost* ash_host = AshWindowTreeHost::Create(params_with_bounds);
  aura::WindowTreeHost* host = ash_host->AsWindowTreeHost();
  if (!input_method_) {   
    input_method_ = ui::CreateInputMethod(this, host->GetAcceleratedWidget());
    input_method_->OnFocus();
    input_method_event_handler_.reset(
        new InputMethodEventHandler(input_method_.get()));
  }
  host->SetSharedInputMethod(input_method_.get());
  ash_host->set_input_method_handler(input_method_event_handler_.get());

  host->window()->SetName(base::StringPrintf(
      "%sRootWindow-%d", params_with_bounds.offscreen ? "Offscreen" : "",
      host_count++));
  host->window()->SetTitle(base::UTF8ToUTF16(display_info.name()));
  host->compositor()->SetBackgroundColor(SK_ColorBLACK);
  host->AddObserver(this);
  InitRootWindowSettings(host->window())->display_id = display.id();
  host->InitHost();

  window_tree_hosts_[display.id()] = ash_host;
  SetDisplayPropertiesOnHost(ash_host, display);

#if defined(OS_CHROMEOS)
  if (switches::ConstrainPointerToRoot())
    ash_host->ConfineCursorToRootWindow();
#endif
  return ash_host;
}
