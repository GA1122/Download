  PathFadeAnimation(OmniboxViewViews* view, SkColor starting_color)
      : view_(view),
        starting_color_(starting_color),
        animation_(
            {
                gfx::MultiAnimation::Part(4000, gfx::Tween::ZERO),
                gfx::MultiAnimation::Part(300, gfx::Tween::FAST_OUT_SLOW_IN),
            },
