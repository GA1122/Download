 static void vnc_async_encoding_start(VncState *orig, VncState *local)
 {
     local->vnc_encoding = orig->vnc_encoding;
     local->features = orig->features;
     local->ds = orig->ds;
      local->vd = orig->vd;
      local->lossy_rect = orig->lossy_rect;
      local->write_pixels = orig->write_pixels;
    local->clientds = orig->clientds;
//     local->client_pf = orig->client_pf;
//     local->client_be = orig->client_be;
      local->tight = orig->tight;
      local->zlib = orig->zlib;
      local->hextile = orig->hextile;
     local->output =  queue->buffer;
     local->csock = -1;  
 
     buffer_reset(&local->output);
 }