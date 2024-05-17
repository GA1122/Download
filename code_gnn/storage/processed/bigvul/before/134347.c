void NewTabButton::GetHitTestMask(HitTestSource source, gfx::Path* path) const {
  DCHECK(path);

  SkScalar w = SkIntToScalar(width());
  SkScalar v_offset = SkIntToScalar(newtab_button_v_offset());

  path->moveTo(0, v_offset + 1);
  path->lineTo(w - 7, v_offset + 1);
  path->lineTo(w - 4, v_offset + 4);
  path->lineTo(w, v_offset + 16);
  path->lineTo(w - 1, v_offset + 17);
  path->lineTo(7, v_offset + 17);
  path->lineTo(4, v_offset + 13);
  path->lineTo(0, v_offset + 1);
  path->close();
}
