   const UsbDeviceHandle::TransferCallback& callback() const {
    return callback_;
//   UsbDeviceHandle::TransferCallback GetCallback() {
//     return base::Bind(&TestCompletionCallback::SetResult,
//                       base::Unretained(this));
    }