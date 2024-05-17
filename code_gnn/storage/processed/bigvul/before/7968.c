static void pixel_format_message (VncState *vs) {
    char pad[3] = { 0, 0, 0 };

    vs->client_pf = qemu_default_pixelformat(32);

    vnc_write_u8(vs, vs->client_pf.bits_per_pixel);  
    vnc_write_u8(vs, vs->client_pf.depth);  

#ifdef HOST_WORDS_BIGENDIAN
    vnc_write_u8(vs, 1);              
#else
    vnc_write_u8(vs, 0);              
#endif
    vnc_write_u8(vs, 1);              
    vnc_write_u16(vs, vs->client_pf.rmax);      
    vnc_write_u16(vs, vs->client_pf.gmax);      
    vnc_write_u16(vs, vs->client_pf.bmax);      
    vnc_write_u8(vs, vs->client_pf.rshift);     
    vnc_write_u8(vs, vs->client_pf.gshift);     
    vnc_write_u8(vs, vs->client_pf.bshift);     
    vnc_write(vs, pad, 3);            

    vnc_hextile_set_pixel_conversion(vs, 0);
    vs->write_pixels = vnc_write_pixels_copy;
}
