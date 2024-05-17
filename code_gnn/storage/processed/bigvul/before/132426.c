UsbDevice::UsbDevice(uint16 vendor_id,
                     uint16 product_id,
                     const base::string16& manufacturer_string,
                     const base::string16& product_string,
                     const base::string16& serial_number)
    : manufacturer_string_(manufacturer_string),
      product_string_(product_string),
      serial_number_(serial_number),
      guid_(base::GenerateGUID()),
      vendor_id_(vendor_id),
      product_id_(product_id) {
}
