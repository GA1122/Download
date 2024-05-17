void UsbTransferFunction::OnCompleted(UsbTransferStatus status,
                                      scoped_refptr<net::IOBuffer> data,
                                      size_t length) {
  scoped_ptr<base::DictionaryValue> transfer_info(new base::DictionaryValue());
  transfer_info->SetInteger(kResultCodeKey, status);
  transfer_info->Set(kDataKey, base::BinaryValue::CreateWithCopiedBuffer(
                                   data->data(), length));

  if (status == device::USB_TRANSFER_COMPLETED) {
    Respond(OneArgument(transfer_info.release()));
  } else {
    scoped_ptr<base::ListValue> error_args(new base::ListValue());
    error_args->Append(transfer_info.release());
    Respond(ErrorWithArguments(error_args.Pass(),
                               ConvertTransferStatusToApi(status)));
  }
}
