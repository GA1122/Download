MojoVideoEncodeAcceleratorService::~MojoVideoEncodeAcceleratorService() {
  DVLOG(1) << __func__;
  DCHECK_CALLED_ON_VALID_SEQUENCE(sequence_checker_);
}
