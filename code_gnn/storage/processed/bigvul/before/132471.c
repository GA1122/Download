base::Value* PopulateDevice(const UsbDevice* device, int id) {
  Device result;
  result.device = id;
  result.vendor_id = device->vendor_id();
  result.product_id = device->product_id();
  return result.ToValue().release();
}
