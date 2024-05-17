const gfx::VectorIcon& GetVectorIconForMediaAction(MediaSessionAction action) {
  switch (action) {
    case MediaSessionAction::kPreviousTrack:
      return kLockScreenPreviousTrackIcon;
    case MediaSessionAction::kPause:
      return kLockScreenPauseIcon;
    case MediaSessionAction::kNextTrack:
      return kLockScreenNextTrackIcon;
    case MediaSessionAction::kPlay:
      return kLockScreenPlayIcon;
    case MediaSessionAction::kSeekBackward:
      return kLockScreenSeekBackwardIcon;
    case MediaSessionAction::kSeekForward:
      return kLockScreenSeekForwardIcon;

    case MediaSessionAction::kStop:
    case MediaSessionAction::kSkipAd:
    case MediaSessionAction::kSeekTo:
    case MediaSessionAction::kScrubTo:
      NOTREACHED();
      break;
  }

  NOTREACHED();
  return gfx::kNoneIcon;
}
