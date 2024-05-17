VolumeBubble::VolumeBubble()
    : SettingLevelBubble(
          ResourceBundle::GetSharedInstance().GetBitmapNamed(
              IDR_VOLUME_BUBBLE_UP_ICON),
          ResourceBundle::GetSharedInstance().GetBitmapNamed(
              IDR_VOLUME_BUBBLE_DOWN_ICON),
          ResourceBundle::GetSharedInstance().GetBitmapNamed(
              IDR_VOLUME_BUBBLE_MUTE_ICON)) {
}
