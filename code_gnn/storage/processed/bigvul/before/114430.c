bool DXVAVideoDecodeAccelerator::InitDecoder() {
  HMODULE decoder_dll = ::GetModuleHandle(L"msmpeg2vdec.dll");
  RETURN_ON_FAILURE(decoder_dll,
                    "msmpeg2vdec.dll required for decoding is not loaded",
                    false);

  typedef HRESULT (WINAPI* GetClassObject)(const CLSID& clsid,
                                           const IID& iid,
                                           void** object);

  GetClassObject get_class_object = reinterpret_cast<GetClassObject>(
      GetProcAddress(decoder_dll, "DllGetClassObject"));
  RETURN_ON_FAILURE(get_class_object,
                    "Failed to get DllGetClassObject pointer", false);

  base::win::ScopedComPtr<IClassFactory> factory;
  HRESULT hr = get_class_object(__uuidof(CMSH264DecoderMFT),
                                __uuidof(IClassFactory),
                                reinterpret_cast<void**>(factory.Receive()));
  RETURN_ON_HR_FAILURE(hr, "DllGetClassObject for decoder failed", false);

  hr = factory->CreateInstance(NULL, __uuidof(IMFTransform),
                               reinterpret_cast<void**>(decoder_.Receive()));
  RETURN_ON_HR_FAILURE(hr, "Failed to create decoder instance", false);

  RETURN_ON_FAILURE(CheckDecoderDxvaSupport(),
                    "Failed to check decoder DXVA support", false);

  hr = decoder_->ProcessMessage(
            MFT_MESSAGE_SET_D3D_MANAGER,
            reinterpret_cast<ULONG_PTR>(device_manager_));
  RETURN_ON_HR_FAILURE(hr, "Failed to pass D3D manager to decoder", false);

  EGLint config_attribs[] = {
    EGL_BUFFER_SIZE, 32,
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
    EGL_ALPHA_SIZE, 0,
    EGL_NONE
  };

  EGLint num_configs;

  if (!eglChooseConfig(
      static_cast<EGLDisplay*>(eglGetDisplay(EGL_DEFAULT_DISPLAY)),
      config_attribs,
      &egl_config_,
      1,
      &num_configs)) {
    return false;
  }

  return SetDecoderMediaTypes();
}
