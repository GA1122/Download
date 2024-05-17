void InspectorPageAgent::FrameScheduledNavigation(
    LocalFrame* frame,
    ScheduledNavigation* scheduled_navigation) {
  GetFrontend()->frameScheduledNavigation(
      IdentifiersFactory::FrameId(frame), scheduled_navigation->Delay(),
      ScheduledNavigationReasonToProtocol(scheduled_navigation->GetReason()),
      scheduled_navigation->Url().GetString());
}
