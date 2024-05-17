  void AnimateToTransforms(
      const PaintedShapeTransforms transforms,
      base::TimeDelta duration,
      ui::LayerAnimator::PreemptionStrategy preemption_strategy,
      gfx::Tween::Type tween,
      ui::LayerAnimationObserver* animation_observer) {
    for (int i = 0; i < PAINTED_SHAPE_COUNT; ++i) {
      ui::LayerAnimator* animator = painted_layers_[i]->GetAnimator();
      ui::ScopedLayerAnimationSettings animation(animator);
      animation.SetPreemptionStrategy(preemption_strategy);
      animation.SetTweenType(tween);
      std::unique_ptr<ui::LayerAnimationElement> element =
          ui::LayerAnimationElement::CreateTransformElement(transforms[i],
                                                            duration);
      ui::LayerAnimationSequence* sequence =
          new ui::LayerAnimationSequence(std::move(element));

      if (animation_observer)
        sequence->AddObserver(animation_observer);

      animator->StartAnimation(sequence);
    }

    {
      ui::ScopedLayerAnimationSettings animation(
          large_shadow_layer_->GetAnimator());
      animation.SetTweenType(tween);
      animation.SetTransitionDuration(duration);

      large_shadow_layer_->SetVisible(true);
    }
  }
