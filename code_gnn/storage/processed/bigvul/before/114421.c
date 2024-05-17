bool DXVAVideoDecodeAccelerator::CreateD3DDevManager() {
  HRESULT hr = Direct3DCreate9Ex(D3D_SDK_VERSION, &d3d9_);
  RETURN_ON_HR_FAILURE(hr, "Direct3DCreate9Ex failed", false);

  D3DPRESENT_PARAMETERS present_params = {0};
  present_params.BackBufferWidth = 1;
  present_params.BackBufferHeight = 1;
  present_params.BackBufferFormat = D3DFMT_UNKNOWN;
  present_params.BackBufferCount = 1;
  present_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
  present_params.hDeviceWindow = ::GetShellWindow();
  present_params.Windowed = TRUE;
  present_params.Flags = D3DPRESENTFLAG_VIDEO;
  present_params.FullScreen_RefreshRateInHz = 0;
  present_params.PresentationInterval = 0;

  hr = d3d9_->CreateDeviceEx(D3DADAPTER_DEFAULT,
                             D3DDEVTYPE_HAL,
                             ::GetShellWindow(),
                             D3DCREATE_FPU_PRESERVE |
                             D3DCREATE_SOFTWARE_VERTEXPROCESSING |
                             D3DCREATE_DISABLE_PSGP_THREADING |
                             D3DCREATE_MULTITHREADED,
                             &present_params,
                             NULL,
                             &device_);
  RETURN_ON_HR_FAILURE(hr, "Failed to create D3D device", false);

  hr = DXVA2CreateDirect3DDeviceManager9(&dev_manager_reset_token_,
                                         &device_manager_);
  RETURN_ON_HR_FAILURE(hr, "DXVA2CreateDirect3DDeviceManager9 failed", false);

  hr = device_manager_->ResetDevice(device_, dev_manager_reset_token_);
  RETURN_ON_HR_FAILURE(hr, "Failed to reset device", false);

  hr = device_->CreateQuery(D3DQUERYTYPE_EVENT, &query_);
  RETURN_ON_HR_FAILURE(hr, "Failed to create D3D device query", false);

  return true;
 }
