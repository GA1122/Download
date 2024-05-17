void MetalayerMode::OnDisable() {
  CommonPaletteTool::OnDisable();

  Shell::Get()->highlighter_controller()->SetEnabled(false);
  activated_via_button_ = false;
}
