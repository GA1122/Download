int vrend_renderer_resource_get_info(int res_handle,
                                     struct vrend_renderer_resource_info *info)
{
   struct vrend_resource *res;
   int elsize;

   if (!info)
      return EINVAL;
   res = vrend_resource_lookup(res_handle, 0);
   if (!res)
      return EINVAL;

   elsize = util_format_get_blocksize(res->base.format);

   info->handle = res_handle;
   info->tex_id = res->id;
   info->width = res->base.width0;
   info->height = res->base.height0;
   info->depth = res->base.depth0;
   info->format = res->base.format;
   info->flags = res->y_0_top ? VIRGL_RESOURCE_Y_0_TOP : 0;
   info->stride = util_format_get_nblocksx(res->base.format, u_minify(res->base.width0, 0)) * elsize;

   return 0;
}
