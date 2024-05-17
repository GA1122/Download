void PictureLayerImpl::AppendQuads(RenderPass* render_pass,
                                   const Occlusion& occlusion_in_content_space,
                                   AppendQuadsData* append_quads_data) {
  DCHECK(!needs_post_commit_initialization_);
  DCHECK_IMPLIES(!raster_source_->GetSize().IsEmpty(),
                 bounds() == raster_source_->GetSize())
      << " bounds " << bounds().ToString() << " pile "
      << raster_source_->GetSize().ToString();

  SharedQuadState* shared_quad_state =
      render_pass->CreateAndAppendSharedQuadState();

  if (raster_source_->IsSolidColor()) {
    PopulateSharedQuadState(shared_quad_state);

    AppendDebugBorderQuad(
        render_pass, bounds(), shared_quad_state, append_quads_data);

    SolidColorLayerImpl::AppendSolidQuads(
        render_pass, occlusion_in_content_space, shared_quad_state,
        visible_content_rect(), raster_source_->GetSolidColor(),
        append_quads_data);
    return;
  }

  float max_contents_scale = MaximumTilingContentsScale();
  gfx::Transform scaled_draw_transform = draw_transform();
  scaled_draw_transform.Scale(SK_MScalar1 / max_contents_scale,
                              SK_MScalar1 / max_contents_scale);
  gfx::Size scaled_content_bounds =
      gfx::ToCeiledSize(gfx::ScaleSize(bounds(), max_contents_scale));
  gfx::Rect scaled_visible_content_rect =
      gfx::ScaleToEnclosingRect(visible_content_rect(), max_contents_scale);
  scaled_visible_content_rect.Intersect(gfx::Rect(scaled_content_bounds));
  Occlusion scaled_occlusion =
      occlusion_in_content_space.GetOcclusionWithGivenDrawTransform(
          scaled_draw_transform);

  shared_quad_state->SetAll(
      scaled_draw_transform, scaled_content_bounds, scaled_visible_content_rect,
      draw_properties().clip_rect, draw_properties().is_clipped,
      draw_properties().opacity, draw_properties().blend_mode,
      sorting_context_id_);

  if (current_draw_mode_ == DRAW_MODE_RESOURCELESS_SOFTWARE) {
    AppendDebugBorderQuad(
        render_pass,
        scaled_content_bounds,
        shared_quad_state,
        append_quads_data,
        DebugColors::DirectPictureBorderColor(),
        DebugColors::DirectPictureBorderWidth(layer_tree_impl()));

    gfx::Rect geometry_rect = scaled_visible_content_rect;
    gfx::Rect opaque_rect = contents_opaque() ? geometry_rect : gfx::Rect();
    gfx::Rect visible_geometry_rect =
        scaled_occlusion.GetUnoccludedContentRect(geometry_rect);
    if (visible_geometry_rect.IsEmpty())
      return;

    gfx::Size texture_size = scaled_visible_content_rect.size();
    gfx::RectF texture_rect = gfx::RectF(texture_size);
    gfx::Rect quad_content_rect = scaled_visible_content_rect;

    PictureDrawQuad* quad =
        render_pass->CreateAndAppendDrawQuad<PictureDrawQuad>();
    quad->SetNew(shared_quad_state, geometry_rect, opaque_rect,
                 visible_geometry_rect, texture_rect, texture_size,
                 nearest_neighbor_, RGBA_8888, quad_content_rect,
                 max_contents_scale, raster_source_);
    return;
  }

  AppendDebugBorderQuad(
      render_pass, scaled_content_bounds, shared_quad_state, append_quads_data);

  if (ShowDebugBorders()) {
    for (PictureLayerTilingSet::CoverageIterator iter(
             tilings_.get(),
             max_contents_scale,
             scaled_visible_content_rect,
             ideal_contents_scale_);
         iter;
         ++iter) {
      SkColor color;
      float width;
      if (*iter && iter->IsReadyToDraw()) {
        ManagedTileState::DrawInfo::Mode mode = iter->draw_info().mode();
        if (mode == ManagedTileState::DrawInfo::SOLID_COLOR_MODE) {
          color = DebugColors::SolidColorTileBorderColor();
          width = DebugColors::SolidColorTileBorderWidth(layer_tree_impl());
        } else if (mode == ManagedTileState::DrawInfo::PICTURE_PILE_MODE) {
          color = DebugColors::PictureTileBorderColor();
          width = DebugColors::PictureTileBorderWidth(layer_tree_impl());
        } else if (iter.resolution() == HIGH_RESOLUTION) {
          color = DebugColors::HighResTileBorderColor();
          width = DebugColors::HighResTileBorderWidth(layer_tree_impl());
        } else if (iter.resolution() == LOW_RESOLUTION) {
          color = DebugColors::LowResTileBorderColor();
          width = DebugColors::LowResTileBorderWidth(layer_tree_impl());
        } else if (iter->contents_scale() > max_contents_scale) {
          color = DebugColors::ExtraHighResTileBorderColor();
          width = DebugColors::ExtraHighResTileBorderWidth(layer_tree_impl());
        } else {
          color = DebugColors::ExtraLowResTileBorderColor();
          width = DebugColors::ExtraLowResTileBorderWidth(layer_tree_impl());
        }
      } else {
        color = DebugColors::MissingTileBorderColor();
        width = DebugColors::MissingTileBorderWidth(layer_tree_impl());
      }

      DebugBorderDrawQuad* debug_border_quad =
          render_pass->CreateAndAppendDrawQuad<DebugBorderDrawQuad>();
      gfx::Rect geometry_rect = iter.geometry_rect();
      gfx::Rect visible_geometry_rect = geometry_rect;
      debug_border_quad->SetNew(shared_quad_state,
                                geometry_rect,
                                visible_geometry_rect,
                                color,
                                width);
    }
  }

  std::vector<PictureLayerTiling*> seen_tilings;

  gfx::Rect scaled_viewport_for_tile_priority = gfx::ScaleToEnclosingRect(
      GetViewportForTilePriorityInContentSpace(), max_contents_scale);

  size_t missing_tile_count = 0u;
  size_t on_demand_missing_tile_count = 0u;
  only_used_low_res_last_append_quads_ = true;
  for (PictureLayerTilingSet::CoverageIterator iter(tilings_.get(),
                                                    max_contents_scale,
                                                    scaled_visible_content_rect,
                                                    ideal_contents_scale_);
       iter;
       ++iter) {
    gfx::Rect geometry_rect = iter.geometry_rect();
    gfx::Rect opaque_rect = contents_opaque() ? geometry_rect : gfx::Rect();
    gfx::Rect visible_geometry_rect =
        scaled_occlusion.GetUnoccludedContentRect(geometry_rect);
    if (visible_geometry_rect.IsEmpty())
      continue;

    append_quads_data->visible_content_area +=
        visible_geometry_rect.width() * visible_geometry_rect.height();

    bool has_draw_quad = false;
    if (*iter && iter->IsReadyToDraw()) {
      const ManagedTileState::DrawInfo& draw_info = iter->draw_info();
      switch (draw_info.mode()) {
        case ManagedTileState::DrawInfo::RESOURCE_MODE: {
          gfx::RectF texture_rect = iter.texture_rect();

          if (iter->contents_scale() != raster_contents_scale_ &&
              iter->contents_scale() != ideal_contents_scale_ &&
              geometry_rect.Intersects(scaled_viewport_for_tile_priority)) {
            append_quads_data->num_incomplete_tiles++;
          }

          TileDrawQuad* quad =
              render_pass->CreateAndAppendDrawQuad<TileDrawQuad>();
          quad->SetNew(shared_quad_state,
                       geometry_rect,
                       opaque_rect,
                       visible_geometry_rect,
                       draw_info.get_resource_id(),
                       texture_rect,
                       iter.texture_size(),
                       draw_info.contents_swizzled(),
                       nearest_neighbor_);
          has_draw_quad = true;
          break;
        }
        case ManagedTileState::DrawInfo::PICTURE_PILE_MODE: {
          if (!layer_tree_impl()
                   ->GetRendererCapabilities()
                   .allow_rasterize_on_demand) {
            ++on_demand_missing_tile_count;
            break;
          }

          gfx::RectF texture_rect = iter.texture_rect();

          ResourceProvider* resource_provider =
              layer_tree_impl()->resource_provider();
          ResourceFormat format =
              resource_provider->memory_efficient_texture_format();
          PictureDrawQuad* quad =
              render_pass->CreateAndAppendDrawQuad<PictureDrawQuad>();
          quad->SetNew(shared_quad_state, geometry_rect, opaque_rect,
                       visible_geometry_rect, texture_rect, iter.texture_size(),
                       nearest_neighbor_, format, iter->content_rect(),
                       iter->contents_scale(), raster_source_);
          has_draw_quad = true;
          break;
        }
        case ManagedTileState::DrawInfo::SOLID_COLOR_MODE: {
          SolidColorDrawQuad* quad =
              render_pass->CreateAndAppendDrawQuad<SolidColorDrawQuad>();
          quad->SetNew(shared_quad_state,
                       geometry_rect,
                       visible_geometry_rect,
                       draw_info.get_solid_color(),
                       false);
          has_draw_quad = true;
          break;
        }
      }
    }

    if (!has_draw_quad) {
      if (draw_checkerboard_for_missing_tiles()) {
        CheckerboardDrawQuad* quad =
            render_pass->CreateAndAppendDrawQuad<CheckerboardDrawQuad>();
        SkColor color = DebugColors::DefaultCheckerboardColor();
        quad->SetNew(
            shared_quad_state, geometry_rect, visible_geometry_rect, color);
      } else {
        SkColor color = SafeOpaqueBackgroundColor();
        SolidColorDrawQuad* quad =
            render_pass->CreateAndAppendDrawQuad<SolidColorDrawQuad>();
        quad->SetNew(shared_quad_state,
                     geometry_rect,
                     visible_geometry_rect,
                     color,
                     false);
      }

      if (geometry_rect.Intersects(scaled_viewport_for_tile_priority)) {
        append_quads_data->num_missing_tiles++;
        ++missing_tile_count;
      }
      append_quads_data->approximated_visible_content_area +=
          visible_geometry_rect.width() * visible_geometry_rect.height();
      continue;
    }

    if (iter.resolution() != HIGH_RESOLUTION) {
      append_quads_data->approximated_visible_content_area +=
          visible_geometry_rect.width() * visible_geometry_rect.height();
    }

    if (iter.resolution() != LOW_RESOLUTION)
      only_used_low_res_last_append_quads_ = false;

    if (seen_tilings.empty() || seen_tilings.back() != iter.CurrentTiling())
      seen_tilings.push_back(iter.CurrentTiling());
  }

  if (missing_tile_count) {
    TRACE_EVENT_INSTANT2("cc",
                         "PictureLayerImpl::AppendQuads checkerboard",
                         TRACE_EVENT_SCOPE_THREAD,
                         "missing_tile_count",
                         missing_tile_count,
                         "on_demand_missing_tile_count",
                         on_demand_missing_tile_count);
  }

  CleanUpTilingsOnActiveLayer(seen_tilings);
}
