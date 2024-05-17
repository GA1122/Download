void vrend_renderer_check_fences(void)
{
   struct vrend_fence *fence, *stor;
   uint32_t latest_id = 0;
   GLenum glret;

   if (!vrend_state.inited)
      return;

   if (vrend_state.sync_thread) {
      flush_eventfd(vrend_state.eventfd);
      pipe_mutex_lock(vrend_state.fence_mutex);
      LIST_FOR_EACH_ENTRY_SAFE(fence, stor, &vrend_state.fence_list, fences) {
         if (fence->fence_id > latest_id)
            latest_id = fence->fence_id;
         free_fence_locked(fence);
      }
      pipe_mutex_unlock(vrend_state.fence_mutex);
   } else {
      vrend_renderer_force_ctx_0();

      LIST_FOR_EACH_ENTRY_SAFE(fence, stor, &vrend_state.fence_list, fences) {
         glret = glClientWaitSync(fence->syncobj, 0, 0);
         if (glret == GL_ALREADY_SIGNALED){
            latest_id = fence->fence_id;
            free_fence_locked(fence);
         }
          
         else if (glret == GL_TIMEOUT_EXPIRED) {
            break;
         }
      }
   }

   if (latest_id == 0)
      return;
   vrend_clicbs->write_fence(latest_id);
}
