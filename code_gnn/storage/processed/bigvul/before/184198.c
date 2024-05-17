 void TranslateInfoBarBase::AnimationProgressed(const ui::Animation* animation) {
    if (background_color_animation_.get() == animation)
      SchedulePaint();   
    else
    InfoBar::AnimationProgressed(animation);
//     InfoBarView::AnimationProgressed(animation);
  }