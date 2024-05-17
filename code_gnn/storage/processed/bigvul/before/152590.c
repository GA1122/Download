  void TestEmulatedSizeDprDsf(int width, int height, float dpr,
                              float compositor_dsf) {
    static base::string16 get_width =
        base::ASCIIToUTF16("Number(window.innerWidth)");
    static base::string16 get_height =
        base::ASCIIToUTF16("Number(window.innerHeight)");
    static base::string16 get_dpr =
        base::ASCIIToUTF16("Number(window.devicePixelRatio * 10)");

    int emulated_width, emulated_height;
    int emulated_dpr;
    blink::WebDeviceEmulationParams params;
    params.view_size.width = width;
    params.view_size.height = height;
    params.device_scale_factor = dpr;
    ReceiveEnableDeviceEmulation(view(), params);
    EXPECT_TRUE(ExecuteJavaScriptAndReturnIntValue(get_width, &emulated_width));
    EXPECT_EQ(width, emulated_width);
    EXPECT_TRUE(ExecuteJavaScriptAndReturnIntValue(get_height,
                                                   &emulated_height));
    EXPECT_EQ(height, emulated_height);
    EXPECT_TRUE(ExecuteJavaScriptAndReturnIntValue(get_dpr, &emulated_dpr));
    EXPECT_EQ(static_cast<int>(dpr * 10), emulated_dpr);
    cc::LayerTreeHost* host =
        view()->GetWidget()->layer_tree_view()->layer_tree_host();
    EXPECT_EQ(compositor_dsf, host->device_scale_factor());
  }
