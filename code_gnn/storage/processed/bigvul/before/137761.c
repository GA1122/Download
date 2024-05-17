  bool MaybeRunCallback() {
    if (wait_for_copy_result || !display_did_finish_frame)
      return false;
    callback.Run(has_damage, main_frame_content_updated, std::move(bitmap));
    return true;
  }
