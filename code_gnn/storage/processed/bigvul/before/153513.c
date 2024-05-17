GM2TabStyle::GM2TabStyle(Tab* tab)
    : tab_(tab),
      hover_controller_(gfx::Animation::ShouldRenderRichAnimation()
                            ? new GlowHoverController(tab)
                            : nullptr) {}
