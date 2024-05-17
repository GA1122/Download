void CompareDrawQuad(DrawQuad* quad,
                     DrawQuad* copy,
                     SharedQuadState* copy_shared_state) {
  EXPECT_EQ(quad->material, copy->material);
  EXPECT_EQ(quad->rect, copy->rect);
  EXPECT_EQ(quad->visible_rect, copy->visible_rect);
  EXPECT_EQ(quad->opaque_rect, copy->opaque_rect);
  EXPECT_EQ(quad->needs_blending, copy->needs_blending);
  EXPECT_EQ(copy_shared_state, copy->shared_quad_state);
}
