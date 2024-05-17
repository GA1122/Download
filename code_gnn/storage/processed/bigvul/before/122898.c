void RenderProcessHostImpl::PropagateBrowserCommandLineToRenderer(
    const CommandLine& browser_cmd,
    CommandLine* renderer_cmd) const {
  static const char* const kSwitchNames[] = {
    switches::kAudioBufferSize,
    switches::kAuditAllHandles,
    switches::kAuditHandles,
    switches::kChromeFrame,
    switches::kDisable3DAPIs,
    switches::kDisableAcceleratedCompositing,
    switches::kDisableAcceleratedVideoDecode,
    switches::kDisableApplicationCache,
    switches::kDisableAudio,
    switches::kDisableAudioOutputResampler,
    switches::kDisableBreakpad,
#if defined(OS_MACOSX)
    switches::kDisableCompositedCoreAnimationPlugins,
#endif
    switches::kDisableDataTransferItems,
    switches::kDisableDatabases,
    switches::kDisableDesktopNotifications,
    switches::kDisableDeviceOrientation,
    switches::kDisableFileSystem,
    switches::kDisableGeolocation,
    switches::kDisableGLMultisampling,
    switches::kDisableGpuVsync,
    switches::kDisableJavaScriptI18NAPI,
    switches::kDisableLocalStorage,
    switches::kDisableLogging,
    switches::kDisableSeccompFilterSandbox,
    switches::kDisableSeccompSandbox,
    switches::kDisableSessionStorage,
    switches::kDisableSharedWorkers,
    switches::kDisableSpeechInput,
#if defined(OS_ANDROID)
    switches::kEnableWebAudio,
#else
    switches::kDisableWebAudio,
#endif
    switches::kEnableWebAudioInput,
    switches::kDisableWebSockets,
    switches::kDomAutomationController,
    switches::kEnableAccessibilityLogging,
    switches::kEnableBrowserPluginCompositing,
    switches::kEnableBrowserPluginForAllViewTypes,
    switches::kEnableDCHECK,
    switches::kEnableEncryptedMedia,
    switches::kEnableExperimentalWebKitFeatures,
    switches::kEnableFixedLayout,
    switches::kEnableDeferredImageDecoding,
    switches::kEnableGPUServiceLogging,
    switches::kEnableGPUClientLogging,
    switches::kEnableGpuBenchmarking,
    switches::kEnableLogging,
    switches::kDisableMediaSource,
    switches::kDisableWebMediaPlayerMS,
#if defined(OS_WIN) || defined(OS_MAC)
    switches::kDisableRendererSideMixing,
#else
    switches::kEnableRendererSideMixing,
#endif
    switches::kEnableStrictSiteIsolation,
    switches::kDisableFullScreen,
    switches::kEnablePepperTesting,
    switches::kEnablePreparsedJsCaching,
    switches::kEnablePruneGpuCommandBuffers,
    switches::kEnablePinch,
#if defined(OS_MACOSX)
    switches::kEnableSandboxLogging,
#endif
    switches::kEnableSeccompSandbox,
    switches::kEnableSoftwareCompositingGLAdapter,
    switches::kEnableStatsTable,
    switches::kEnableThreadedCompositing,
    switches::kEnableTouchDragDrop,
    switches::kDisableThreadedCompositing,
    switches::kDisableTouchAdjustment,
    switches::kEnableViewport,
    switches::kForceDeviceScaleFactor,
    switches::kFullMemoryCrashReport,
#if !defined (GOOGLE_CHROME_BUILD)
    switches::kInProcessPlugins,
#endif   
    switches::kInProcessWebGL,
    switches::kJavaScriptFlags,
    switches::kLoggingLevel,
#if defined(OS_ANDROID)
    switches::kMediaPlayerInRenderProcess,
    switches::kNetworkCountryIso,
#endif
    switches::kNoReferrers,
    switches::kNoSandbox,
    switches::kOldCheckboxStyle,
    switches::kPpapiOutOfProcess,
    switches::kRegisterPepperPlugins,
    switches::kRendererAssertTest,
#if defined(OS_POSIX)
    switches::kChildCleanExit,
#endif
    switches::kRendererStartupDialog,
    switches::kShowPaintRects,
    switches::kSitePerProcess,
    switches::kTestSandbox,
    switches::kTouchEvents,
    switches::kTraceStartup,
    switches::kUseGL,
    switches::kUseMobileUserAgent,
    switches::kUserAgent,
    switches::kV,
    switches::kVideoThreads,
    switches::kVModule,
    switches::kWebCoreLogChannels,
    cc::switches::kBackgroundColorInsteadOfCheckerboard,
    cc::switches::kEnableImplSidePainting,
    cc::switches::kEnablePartialSwap,
    cc::switches::kNumRasterThreads,
    cc::switches::kShowPropertyChangedRects,
    cc::switches::kShowSurfaceDamageRects,
    cc::switches::kShowScreenSpaceRects,
    cc::switches::kShowReplicaScreenSpaceRects,
    cc::switches::kShowNonOccludingRects,
    cc::switches::kShowOccludingRects,
    cc::switches::kTraceOverdraw,
  };
  renderer_cmd->CopySwitchesFrom(browser_cmd, kSwitchNames,
                                 arraysize(kSwitchNames));

  if (GetBrowserContext()->IsOffTheRecord() &&
      !browser_cmd.HasSwitch(switches::kDisableDatabases)) {
    renderer_cmd->AppendSwitch(switches::kDisableDatabases);
#if defined(OS_ANDROID)
    renderer_cmd->AppendSwitch(switches::kDisableMediaHistoryLogging);
#endif
  }
}
