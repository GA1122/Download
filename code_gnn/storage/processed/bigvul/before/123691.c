void GpuVideoDecodeAccelerator::Initialize(
    const media::VideoCodecProfile profile,
    IPC::Message* init_done_msg) {
  DCHECK(!video_decode_accelerator_.get());
  DCHECK(!init_done_msg_);
  DCHECK(init_done_msg);
  init_done_msg_ = init_done_msg;
  if (!stub_)
    return;

#if !defined(OS_WIN)
  if (!make_context_current_.Run()) {
    NotifyError(media::VideoDecodeAccelerator::PLATFORM_FAILURE);
    return;
  }
#endif

#if defined(OS_WIN)
  if (base::win::GetVersion() < base::win::VERSION_WIN7) {
    NOTIMPLEMENTED() << "HW video decode acceleration not available.";
    NotifyError(media::VideoDecodeAccelerator::PLATFORM_FAILURE);
    return;
  }
  DLOG(INFO) << "Initializing DXVA HW decoder for windows.";
  video_decode_accelerator_.reset(new DXVAVideoDecodeAccelerator(
      this, make_context_current_));
#elif defined(OS_CHROMEOS) && defined(ARCH_CPU_ARMEL)
  video_decode_accelerator_.reset(new OmxVideoDecodeAccelerator(
      gfx::GLSurfaceEGL::GetHardwareDisplay(),
      stub_->decoder()->GetGLContext()->GetHandle(),
      this,
      make_context_current_));
#elif defined(OS_CHROMEOS) && defined(ARCH_CPU_X86_FAMILY)
  gfx::GLContextGLX* glx_context =
      static_cast<gfx::GLContextGLX*>(stub_->decoder()->GetGLContext());
  GLXContext glx_context_handle =
      static_cast<GLXContext>(glx_context->GetHandle());
  video_decode_accelerator_.reset(new VaapiVideoDecodeAccelerator(
      glx_context->display(), glx_context_handle, this,
      make_context_current_));
#elif defined(OS_MACOSX)
  video_decode_accelerator_.reset(new MacVideoDecodeAccelerator(
      static_cast<CGLContextObj>(
          stub_->decoder()->GetGLContext()->GetHandle()),
      this));
#else
  NOTIMPLEMENTED() << "HW video decode acceleration not available.";
  NotifyError(media::VideoDecodeAccelerator::PLATFORM_FAILURE);
  return;
#endif

  if (!video_decode_accelerator_->Initialize(profile))
    NotifyError(media::VideoDecodeAccelerator::PLATFORM_FAILURE);
 }
