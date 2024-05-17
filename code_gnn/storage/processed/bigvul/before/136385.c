static bool EffectGroupContainsChunk(
    const EffectPaintPropertyNode& group_effect,
    const PaintChunk& chunk) {
  const auto* effect = chunk.properties.Effect();
  return effect == &group_effect ||
         StrictChildOfAlongPath(&group_effect, effect);
}
