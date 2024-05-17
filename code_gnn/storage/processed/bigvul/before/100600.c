  void AddPaintLayer(PaintedShape painted_shape) {
    ui::LayerDelegate* delegate = nullptr;
    switch (painted_shape) {
      case TOP_LEFT_CIRCLE:
      case TOP_RIGHT_CIRCLE:
      case BOTTOM_RIGHT_CIRCLE:
      case BOTTOM_LEFT_CIRCLE:
        delegate = circle_layer_delegate_.get();
        break;
      case HORIZONTAL_RECT:
      case VERTICAL_RECT:
        delegate = rect_layer_delegate_.get();
        break;
      case PAINTED_SHAPE_COUNT:
        NOTREACHED() << "PAINTED_SHAPE_COUNT is not an actual shape type.";
        break;
    }

    ui::Layer* layer = new ui::Layer();
    Add(layer);

    layer->SetBounds(gfx::Rect(small_size_));
    layer->SetFillsBoundsOpaquely(false);
    layer->set_delegate(delegate);
    layer->SetVisible(true);
    layer->SetOpacity(1.0);
    layer->SetMasksToBounds(false);
    layer->set_name("PAINTED_SHAPE_COUNT:" + ToLayerName(painted_shape));

    painted_layers_[static_cast<int>(painted_shape)].reset(layer);
  }
