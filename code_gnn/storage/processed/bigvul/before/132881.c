  void SetupDrawPropertiesAndUpdateTiles(FakePictureLayerImpl* layer,
                                         float ideal_contents_scale,
                                         float device_scale_factor,
                                         float page_scale_factor,
                                         float maximum_animation_contents_scale,
                                         bool animating_transform_to_screen) {
    layer->draw_properties().ideal_contents_scale = ideal_contents_scale;
    layer->draw_properties().device_scale_factor = device_scale_factor;
    layer->draw_properties().page_scale_factor = page_scale_factor;
    layer->draw_properties().maximum_animation_contents_scale =
        maximum_animation_contents_scale;
    layer->draw_properties().screen_space_transform_is_animating =
        animating_transform_to_screen;
    bool resourceless_software_draw = false;
    layer->UpdateTiles(Occlusion(), resourceless_software_draw);
  }
