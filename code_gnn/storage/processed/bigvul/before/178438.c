 int gs_lib_ctx_init( gs_memory_t *mem )
 {
     gs_lib_ctx_t *pio = 0;
 
      
     if (mem == 0 || mem != mem->non_gc_memory)
         return_error(gs_error_Fatal);
 
 #ifndef GS_THREADSAFE
     mem_err_print = mem;
 #endif
 
     if (mem->gs_lib_ctx)  
         return 0;
 
     pio = (gs_lib_ctx_t*)gs_alloc_bytes_immovable(mem,
                                                   sizeof(gs_lib_ctx_t),
                                                   "gs_lib_ctx_init");
     if( pio == 0 )
         return -1;
 
      
     memset(pio, 0, sizeof(*pio));
      
     pio->memory               = mem;
     gs_lib_ctx_get_real_stdio(&pio->fstdin, &pio->fstdout, &pio->fstderr );
     pio->stdin_is_interactive = true;
      
     pio->gs_next_id           = 5;   
 
      
     mem->gs_lib_ctx = pio;
      
     pio->profiledir = NULL;
     pio->profiledir_len = 0;
     gs_lib_ctx_set_icc_directory(mem, DEFAULT_DIR_ICC, strlen(DEFAULT_DIR_ICC));
 
     if (gs_lib_ctx_set_default_device_list(mem, gs_dev_defaults,
                         strlen(gs_dev_defaults)) < 0) {
         
         gs_free_object(mem, pio, "gs_lib_ctx_init");
         mem->gs_lib_ctx = NULL;
     }
 
      
     if (gscms_create(mem)) {
 Failure:
         gs_free_object(mem, mem->gs_lib_ctx->default_device_list,
                 "gs_lib_ctx_fin");
 
         gs_free_object(mem, pio, "gs_lib_ctx_init");
         mem->gs_lib_ctx = NULL;
         return -1;
     }
 
      
     if (sjpxd_create(mem)) {
          gscms_destroy(mem);
          goto Failure;
      }
//     pio->client_check_file_permission = NULL;
      gp_get_realtime(pio->real_time_0);
  
       
     pio->scanconverter = GS_SCANCONVERTER_DEFAULT;
 
     return 0;
 }