void PictureLayerImplTest::TestQuadsForSolidColor(bool test_for_solid) {
  base::TimeTicks time_ticks;
  time_ticks += base::TimeDelta::FromMilliseconds(1);
  host_impl_.SetCurrentBeginFrameArgs(
      CreateBeginFrameArgsForTesting(BEGINFRAME_FROM_HERE, time_ticks));

  gfx::Size tile_size(100, 100);
  gfx::Size layer_bounds(200, 200);
  gfx::Rect layer_rect(layer_bounds);

  FakeContentLayerClient client;
  scoped_refptr<PictureLayer> layer = PictureLayer::Create(&client);
  FakeLayerTreeHostClient host_client(FakeLayerTreeHostClient::DIRECT_3D);
  scoped_ptr<FakeLayerTreeHost> host = FakeLayerTreeHost::Create(&host_client);
  host->SetRootLayer(layer);
  RecordingSource* recording_source = layer->GetRecordingSourceForTesting();

  int frame_number = 0;

  client.set_fill_with_nonsolid_color(!test_for_solid);

  Region invalidation(layer_rect);
  recording_source->UpdateAndExpandInvalidation(
      &client, &invalidation, false, layer_bounds, layer_rect, frame_number++,
      Picture::RECORD_NORMALLY);

  scoped_refptr<RasterSource> pending_raster_source =
      recording_source->CreateRasterSource();

  SetupPendingTreeWithFixedTileSize(pending_raster_source, tile_size, Region());
  ActivateTree();

  if (test_for_solid) {
    EXPECT_EQ(0u, active_layer_->tilings()->num_tilings());
  } else {
    ASSERT_TRUE(active_layer_->tilings());
    ASSERT_GT(active_layer_->tilings()->num_tilings(), 0u);
    std::vector<Tile*> tiles =
        active_layer_->tilings()->tiling_at(0)->AllTilesForTesting();
    EXPECT_FALSE(tiles.empty());
    host_impl_.tile_manager()->InitializeTilesWithResourcesForTesting(tiles);
  }

  scoped_ptr<RenderPass> render_pass = RenderPass::Create();
  AppendQuadsData data;
  active_layer_->WillDraw(DRAW_MODE_SOFTWARE, nullptr);
  active_layer_->AppendQuads(render_pass.get(), Occlusion(), &data);
  active_layer_->DidDraw(nullptr);

  DrawQuad::Material expected = test_for_solid
                                    ? DrawQuad::Material::SOLID_COLOR
                                    : DrawQuad::Material::TILED_CONTENT;
  EXPECT_EQ(expected, render_pass->quad_list.front()->material);
}
