void CreateJpegDecodeAccelerator(
    media::mojom::JpegDecodeAcceleratorRequest request) {
  BrowserThread::PostTask(BrowserThread::IO, FROM_HERE,
                          base::BindOnce(&CreateJpegDecodeAcceleratorOnIOThread,
                                         std::move(request)));
}
