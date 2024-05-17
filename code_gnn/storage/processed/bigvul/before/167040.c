void MetalayerMode::OnEnable() {
  CommonPaletteTool::OnEnable();

  Shell::Get()->highlighter_controller()->SetExitCallback(
      base::BindOnce(&MetalayerMode::OnMetalayerSessionComplete,
                     weak_factory_.GetWeakPtr()),
      !activated_via_button_  );
  Shell::Get()->highlighter_controller()->SetEnabled(true);
  delegate()->HidePalette();
}
