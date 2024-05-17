bool VRDisplay::getFrameData(VRFrameData* frame_data) {
  if (!IsPresentationFocused() || !frame_pose_ || display_blurred_)
    return false;

  if (!frame_data)
    return false;

  if (depth_near_ == depth_far_)
    return false;

  return frame_data->Update(frame_pose_, eye_parameters_left_,
                            eye_parameters_right_, depth_near_, depth_far_);
}
