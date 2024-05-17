void ConversionContext::StartEffect(const EffectPaintPropertyNode* effect) {
  if (effect->OutputClip())
    SwitchToClip(effect->OutputClip());
  else
    EndClips();

  int saved_count = 0;
  size_t save_layer_id = kNotFound;

  SwitchToTransform(effect->LocalTransformSpace());

  bool has_filter = !effect->Filter().IsEmpty();
  bool has_opacity = effect->Opacity() != 1.f;
  bool has_other_effects = effect->BlendMode() != SkBlendMode::kSrcOver ||
                           effect->GetColorFilter() != kColorFilterNone;
  DCHECK(!has_filter || !(has_opacity || has_other_effects));

  cc_list_.StartPaint();
  if (!has_filter) {
    auto alpha =
        static_cast<uint8_t>(gfx::ToFlooredInt(255 * effect->Opacity()));
    if (has_other_effects) {
      cc::PaintFlags flags;
      flags.setBlendMode(effect->BlendMode());
      flags.setAlpha(alpha);
      flags.setColorFilter(GraphicsContext::WebCoreColorFilterToSkiaColorFilter(
          effect->GetColorFilter()));
      save_layer_id = cc_list_.push<cc::SaveLayerOp>(nullptr, &flags);
    } else {
      constexpr bool preserve_lcd_text_requests = false;
      save_layer_id = cc_list_.push<cc::SaveLayerAlphaOp>(
          nullptr, alpha, preserve_lcd_text_requests);
    }
    saved_count++;
  } else {
    FloatPoint filter_origin = effect->PaintOffset();
    if (filter_origin != FloatPoint()) {
      cc_list_.push<cc::SaveOp>();
      cc_list_.push<cc::TranslateOp>(filter_origin.X(), filter_origin.Y());
      saved_count++;
    }

    gfx::SizeF empty;
    cc::PaintFlags filter_flags;
    filter_flags.setImageFilter(cc::RenderSurfaceFilters::BuildImageFilter(
        effect->Filter().AsCcFilterOperations(), empty));
    save_layer_id = cc_list_.push<cc::SaveLayerOp>(nullptr, &filter_flags);
    saved_count++;

    if (filter_origin != FloatPoint())
      cc_list_.push<cc::TranslateOp>(-filter_origin.X(), -filter_origin.Y());
  }
  cc_list_.EndPaintOfPairedBegin();

  DCHECK_GT(saved_count, 0);
  DCHECK_LE(saved_count, 2);
  DCHECK_NE(save_layer_id, kNotFound);

  const ClipPaintPropertyNode* input_clip = current_clip_;
  PushState(StateEntry::kEffect, saved_count);
  effect_bounds_stack_.emplace_back(
      EffectBoundsInfo{save_layer_id, current_transform_});
  current_clip_ = input_clip;
  current_effect_ = effect;
}
