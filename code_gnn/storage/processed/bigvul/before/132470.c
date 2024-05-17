base::Value* PopulateConnectionHandle(int handle,
                                      int vendor_id,
                                      int product_id) {
  ConnectionHandle result;
  result.handle = handle;
  result.vendor_id = vendor_id;
  result.product_id = product_id;
  return result.ToValue().release();
}
