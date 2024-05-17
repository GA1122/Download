PepperVideoRenderer3D::PepperVideoRenderer3D()
    : event_handler_(nullptr),
      latest_input_event_timestamp_(0),
      initialization_finished_(false),
      decode_pending_(false),
      get_picture_pending_(false),
      paint_pending_(false),
      latest_frame_id_(0),
      force_repaint_(false),
      current_shader_program_texture_target_(0),
      shader_program_(0),
      shader_texcoord_scale_location_(0),
      callback_factory_(this) {
}
