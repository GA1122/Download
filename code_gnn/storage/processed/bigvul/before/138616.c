  scoped_refptr<SiteInstance> ConvertToSiteInstance(
      RenderFrameHostManager* rfhm,
      const SiteInstanceDescriptor& descriptor,
      SiteInstance* candidate_instance) {
    return rfhm->ConvertToSiteInstance(descriptor, candidate_instance);
  }
