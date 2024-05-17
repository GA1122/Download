std::unique_ptr<ui::LayerTreeOwner> CreateLayerTreeForSnapshot(
    aura::Window* root_window) {
  LayerSet blocked_layers;
  for (auto* browser : *BrowserList::GetInstance()) {
    if (browser->profile()->IsOffTheRecord())
      blocked_layers.insert(browser->window()->GetNativeWindow()->layer());
  }

  LayerSet excluded_layers;
  if (chromeos::GetAshConfig() != ash::Config::MASH) {
    aura::Window* overlay_container = ash::Shell::GetContainer(
        root_window, ash::kShellWindowId_OverlayContainer);
    if (overlay_container != nullptr)
      excluded_layers.insert(overlay_container->layer());
  }

  auto layer_tree_owner = ::wm::RecreateLayersWithClosure(
      root_window, base::BindRepeating(
                       [](LayerSet blocked_layers, LayerSet excluded_layers,
                          ui::LayerOwner* owner) -> std::unique_ptr<ui::Layer> {
                         if (blocked_layers.count(owner->layer()->parent()))
                           return nullptr;
                         if (blocked_layers.count(owner->layer())) {
                           auto layer = std::make_unique<ui::Layer>(
                               ui::LayerType::LAYER_SOLID_COLOR);
                           layer->SetBounds(owner->layer()->bounds());
                           layer->SetColor(SK_ColorBLACK);
                           return layer;
                         }
                         if (excluded_layers.count(owner->layer()))
                           return nullptr;
                         return owner->RecreateLayer();
                       },
                       std::move(blocked_layers), std::move(excluded_layers)));

  DCHECK(layer_tree_owner);

  auto* root_layer = layer_tree_owner->root();
  root_layer->SetTransform(gfx::Transform());
  root_window->layer()->Add(root_layer);
  root_window->layer()->StackAtBottom(root_layer);
  return layer_tree_owner;
}
