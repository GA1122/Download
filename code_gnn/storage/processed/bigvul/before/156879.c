WebInsecureRequestPolicy DocumentInit::GetInsecureRequestPolicy() const {
  DCHECK(MasterDocumentLoader());
  Frame* parent_frame = MasterDocumentLoader()->GetFrame()->Tree().Parent();
  if (!parent_frame)
    return kLeaveInsecureRequestsAlone;
  return parent_frame->GetSecurityContext()->GetInsecureRequestPolicy();
}
