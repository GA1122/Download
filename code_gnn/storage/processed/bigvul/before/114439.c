void DXVAVideoDecodeAccelerator::PreSandboxInitialization() {
  DCHECK(!pre_sandbox_init_done_);

  static wchar_t* decoding_dlls[] = {
    L"d3d9.dll",
    L"d3dx9_43.dll",
    L"dxva2.dll",
    L"mf.dll",
    L"mfplat.dll",
    L"msmpeg2vdec.dll",
  };

  for (int i = 0; i < arraysize(decoding_dlls); ++i) {
    if (!::LoadLibrary(decoding_dlls[i])) {
      DLOG(ERROR) << "Failed to load decoder dll: " << decoding_dlls[i]
                  << ", Error: " << ::GetLastError();
      return;
    }
  }

  RETURN_ON_FAILURE(CreateD3DDevManager(),
                    "Failed to initialize D3D device and manager",);
  pre_sandbox_init_done_ = true;
}
