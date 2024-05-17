static void AppendQuadsToFillScreen(const gfx::Rect& root_scroll_layer_rect,
                                    RenderPass* target_render_pass,
                                    RenderSurfaceImpl* root_render_surface,
                                    SkColor screen_background_color,
                                    const Region& fill_region) {
  if (!root_render_surface || !SkColorGetA(screen_background_color))
    return;
  if (fill_region.IsEmpty())
    return;


  gfx::Rect root_target_rect = root_render_surface->content_rect();
  float opacity = 1.f;
  int sorting_context_id = 0;
  SharedQuadState* shared_quad_state =
      target_render_pass->CreateAndAppendSharedQuadState();
  shared_quad_state->SetAll(gfx::Transform(), root_target_rect.size(),
                            root_target_rect, root_target_rect, false, opacity,
                            SkBlendMode::kSrcOver, sorting_context_id);

  for (Region::Iterator fill_rects(fill_region); fill_rects.has_rect();
       fill_rects.next()) {
    gfx::Rect screen_space_rect = fill_rects.rect();
    gfx::Rect visible_screen_space_rect = screen_space_rect;
    SolidColorDrawQuad* quad =
        target_render_pass->CreateAndAppendDrawQuad<SolidColorDrawQuad>();
    quad->SetNew(shared_quad_state, screen_space_rect,
                 visible_screen_space_rect, screen_background_color, false);
  }
}
