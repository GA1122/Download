SettingLevelBubble::SettingLevelBubble(SkBitmap* increase_icon,
                                       SkBitmap* decrease_icon,
                                       SkBitmap* disabled_icon)
    :  current_percent_(-1.0),
       target_percent_(-1.0),
       increase_icon_(increase_icon),
       decrease_icon_(decrease_icon),
       disabled_icon_(disabled_icon),
       view_(NULL),
       is_animating_(false) {
}
