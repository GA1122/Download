TransportDIB* BrowserPluginGuest::GetDamageBufferFromEmbedder(
    RenderViewHost* embedder_rvh,
    const BrowserPluginHostMsg_ResizeGuest_Params& params) {
  TransportDIB* damage_buffer = NULL;
#if defined(OS_WIN)
  HANDLE section;
  DuplicateHandle(embedder_rvh->GetProcess()->GetHandle(),
                  params.damage_buffer_id.handle,
                  GetCurrentProcess(),
                  &section,
                  STANDARD_RIGHTS_REQUIRED | FILE_MAP_READ | FILE_MAP_WRITE,
                  FALSE,
                  0);
  damage_buffer = TransportDIB::Map(section);
#elif defined(OS_MACOSX)
  damage_buffer = TransportDIB::Map(params.damage_buffer_handle);
#elif defined(OS_ANDROID)
  damage_buffer = TransportDIB::Map(params.damage_buffer_id);
#elif defined(OS_POSIX)
  damage_buffer = TransportDIB::Map(params.damage_buffer_id.shmkey);
#endif   
  DCHECK(damage_buffer);
  return damage_buffer;
}
