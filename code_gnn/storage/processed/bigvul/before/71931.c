MagickExport void DestroyModuleList(void)
{
   
  LockSemaphoreInfo(module_semaphore);
#if defined(MAGICKCORE_MODULES_SUPPORT)
  if (module_list != (SplayTreeInfo *) NULL)
    module_list=DestroySplayTree(module_list);
#endif
  UnlockSemaphoreInfo(module_semaphore);
}
