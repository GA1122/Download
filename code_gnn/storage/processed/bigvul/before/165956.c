void CopyConstraintsIntoRtcConfiguration(
    const blink::WebMediaConstraints constraints,
    webrtc::PeerConnectionInterface::RTCConfiguration* configuration) {
  if (constraints.IsEmpty()) {
    return;
  }

  bool the_value;
  if (GetConstraintValueAsBoolean(
          constraints, &blink::WebMediaTrackConstraintSet::enable_i_pv6,
          &the_value)) {
    configuration->disable_ipv6 = !the_value;
  } else {
    configuration->disable_ipv6 = false;
  }

  if (GetConstraintValueAsBoolean(
          constraints, &blink::WebMediaTrackConstraintSet::enable_dscp,
          &the_value)) {
    configuration->set_dscp(the_value);
  }

  if (GetConstraintValueAsBoolean(
          constraints,
          &blink::WebMediaTrackConstraintSet::goog_cpu_overuse_detection,
          &the_value)) {
    configuration->set_cpu_adaptation(the_value);
  }

  if (GetConstraintValueAsBoolean(
          constraints,
          &blink::WebMediaTrackConstraintSet::
              goog_enable_video_suspend_below_min_bitrate,
          &the_value)) {
    configuration->set_suspend_below_min_bitrate(the_value);
  }

  if (!GetConstraintValueAsBoolean(
          constraints,
          &blink::WebMediaTrackConstraintSet::enable_rtp_data_channels,
          &configuration->enable_rtp_data_channel)) {
    configuration->enable_rtp_data_channel = false;
  }
  int rate;
  if (GetConstraintValueAsInteger(
          constraints,
          &blink::WebMediaTrackConstraintSet::goog_screencast_min_bitrate,
          &rate)) {
    configuration->screencast_min_bitrate = rate;
  }
  configuration->combined_audio_video_bwe = ConstraintToOptional(
      constraints,
      &blink::WebMediaTrackConstraintSet::goog_combined_audio_video_bwe);
  configuration->enable_dtls_srtp = ConstraintToOptional(
      constraints, &blink::WebMediaTrackConstraintSet::enable_dtls_srtp);
}
