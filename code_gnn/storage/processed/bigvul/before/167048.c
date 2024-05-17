MetalayerMode::~MetalayerMode() {
  Shell::Get()->voice_interaction_controller()->RemoveObserver(this);
  Shell::Get()->RemovePreTargetHandler(this);
}
