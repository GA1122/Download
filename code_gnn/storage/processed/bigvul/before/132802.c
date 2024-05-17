PictureLayer::PictureLayer(ContentLayerClient* client)
    : client_(client),
      instrumentation_object_tracker_(id()),
      update_source_frame_number_(-1),
      can_use_lcd_text_for_update_(true),
      is_mask_(false),
      nearest_neighbor_(false) {
}
