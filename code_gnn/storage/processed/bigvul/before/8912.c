int vrend_renderer_resource_attach_iov(int res_handle, struct iovec *iov,
                                       int num_iovs)
{
   struct vrend_resource *res;

   res = vrend_resource_lookup(res_handle, 0);
   if (!res)
      return EINVAL;

   if (res->iov)
      return 0;

    
   res->iov = iov;
   res->num_iovs = num_iovs;
   return 0;
}
