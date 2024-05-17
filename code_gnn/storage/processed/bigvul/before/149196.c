  void SimulateMediaSessionChanged(
      media_session::mojom::MediaPlaybackState playback_state) {
    media_controls_view_->MediaSessionChanged(base::UnguessableToken::Create());

    media_session::mojom::MediaSessionInfoPtr session_info(
        media_session::mojom::MediaSessionInfo::New());
    session_info->playback_state = playback_state;

    media_controls_view_->MediaSessionInfoChanged(session_info.Clone());
  }
