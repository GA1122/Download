void RenderWidgetHostViewAura::CreateAuraWindow(ui::wm::WindowType type) {
  DCHECK(!window_);
  window_ = new aura::Window(this);
  window_->SetName("RenderWidgetHostViewAura");
  window_->SetProperty(aura::client::kEmbedType,
                       aura::client::WindowEmbedType::EMBED_IN_OWNER);
  event_handler_->set_window(window_);
  window_observer_.reset(new WindowObserver(this));

  aura::client::SetTooltipText(window_, &tooltip_);
  aura::client::SetActivationDelegate(window_, this);
  aura::client::SetFocusChangeObserver(window_, this);
  display::Screen::GetScreen()->AddObserver(this);

  window_->SetType(type);
  window_->Init(ui::LAYER_SOLID_COLOR);
  window_->layer()->SetColor(background_color_);

  if (!IsMus())
    return;

  mojom::RenderWidgetWindowTreeClientFactoryPtr factory;
  BindInterface(host_->GetProcess(), &factory);

  ui::mojom::WindowTreeClientPtr window_tree_client;
  factory->CreateWindowTreeClientForRenderWidget(
      host_->GetRoutingID(), mojo::MakeRequest(&window_tree_client));
  aura::WindowPortMus::Get(window_)->Embed(
      std::move(window_tree_client),
      ui::mojom::kEmbedFlagEmbedderInterceptsEvents,
      base::Bind(&EmbedCallback));
}
