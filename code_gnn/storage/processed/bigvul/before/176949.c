bool InputDispatcher::hasInjectionPermission(int32_t injectorPid, int32_t injectorUid) {
 return injectorUid == 0
 || mPolicy->checkInjectEventsPermissionNonReentrant(injectorPid, injectorUid);
}
