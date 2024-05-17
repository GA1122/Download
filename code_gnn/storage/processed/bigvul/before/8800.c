static void free_fence_locked(struct vrend_fence *fence)
{
   list_del(&fence->fences);
   glDeleteSync(fence->syncobj);
   free(fence);
}
