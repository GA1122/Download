void GpuProcessHost::InitFontRenderParamsOnIO(
    const gfx::FontRenderParams& params) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  DCHECK(!GetFontRenderParamsOnIO().initialized);

  GetFontRenderParamsOnIO().initialized = true;
  GetFontRenderParamsOnIO().params = params;
}
