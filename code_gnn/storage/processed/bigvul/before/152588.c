  void SetDeviceScaleFactor(float dsf) {
    VisualProperties visual_properties;
    visual_properties.screen_info.device_scale_factor = dsf;
    visual_properties.new_size = gfx::Size(100, 100);
    visual_properties.compositor_viewport_pixel_size = gfx::Size(200, 200);
    visual_properties.visible_viewport_size = visual_properties.new_size;
    visual_properties.auto_resize_enabled =
        view()->GetWidget()->auto_resize_mode();
    visual_properties.capture_sequence_number =
        view()->GetWidget()->capture_sequence_number();
    visual_properties.min_size_for_auto_resize =
        view()->GetWidget()->min_size_for_auto_resize();
    visual_properties.max_size_for_auto_resize =
        view()->GetWidget()->max_size_for_auto_resize();
    visual_properties.local_surface_id_allocation =
        viz::LocalSurfaceIdAllocation(
            viz::LocalSurfaceId(1, 1, base::UnguessableToken::Create()),
            base::TimeTicks::Now());
    view()->GetWidget()->OnSynchronizeVisualProperties(visual_properties);
    ASSERT_EQ(dsf, view()->GetWidget()->GetWebScreenInfo().device_scale_factor);
    ASSERT_EQ(dsf,
              view()->GetWidget()->GetOriginalScreenInfo().device_scale_factor);
  }
