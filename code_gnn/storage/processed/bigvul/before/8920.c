int vrend_renderer_transfer_iov(const struct vrend_transfer_info *info,
                                int transfer_mode)
{
   struct vrend_resource *res;
   struct vrend_context *ctx;
   struct iovec *iov;
   int num_iovs;

   if (!info->box)
      return EINVAL;

   ctx = vrend_lookup_renderer_ctx(info->ctx_id);
   if (!ctx)
      return EINVAL;

   if (info->ctx_id == 0)
      res = vrend_resource_lookup(info->handle, 0);
   else
      res = vrend_renderer_ctx_res_lookup(ctx, info->handle);

   if (!res) {
      if (info->ctx_id)
         report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_RESOURCE, info->handle);
      return EINVAL;
   }

   iov = info->iovec;
   num_iovs = info->iovec_cnt;

   if (res->iov && (!iov || num_iovs == 0)) {
      iov = res->iov;
      num_iovs = res->num_iovs;
   }

   if (!iov) {
      if (info->ctx_id)
         report_context_error(ctx, VIRGL_ERROR_CTX_ILLEGAL_RESOURCE, info->handle);
      return EINVAL;
   }

   if (!check_transfer_bounds(res, info))
      return EINVAL;

   if (!check_iov_bounds(res, info, iov, num_iovs))
      return EINVAL;

   vrend_hw_switch_context(vrend_lookup_renderer_ctx(0), true);

   if (transfer_mode == VREND_TRANSFER_WRITE)
      return vrend_renderer_transfer_write_iov(ctx, res, iov, num_iovs,
                                               info);
   else
      return vrend_renderer_transfer_send_iov(ctx, res, iov, num_iovs,
                                              info);
}
