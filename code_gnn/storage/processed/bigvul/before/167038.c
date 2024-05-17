MetalayerMode::MetalayerMode(Delegate* delegate)
    : CommonPaletteTool(delegate), weak_factory_(this) {
  Shell::Get()->AddPreTargetHandler(this);
  Shell::Get()->voice_interaction_controller()->AddObserver(this);
}
