#include "processor.h"
#include "controller.h"
#include "ids.h"
#include "version.h"

#include "public.sdk/source/main/pluginfactory.h"
#include "pluginterfaces/vst/ivstaudioprocessor.h"

using namespace Steinberg;
using namespace Steinberg::Vst;

#define stringPluginName "125A Noctomorph"

BEGIN_FACTORY_DEF(stringCompanyName, stringCompanyWeb, stringCompanyEmail)

DEF_CLASS2(INLINE_UID_FROM_FUID(Noctomorph::ProcessorUID),
           PClassInfo::kManyInstances,
           kVstAudioEffectClass,
           stringPluginName,
           Vst::kDistributable,
           "Instrument|Synth",
           FULL_VERSION_STR,
           kVstVersionString,
           Noctomorph::Processor::createInstance)

DEF_CLASS2(INLINE_UID_FROM_FUID(Noctomorph::ControllerUID),
           PClassInfo::kManyInstances,
           kVstComponentControllerClass,
           stringPluginName " Controller",
           0,
           "",
           FULL_VERSION_STR,
           kVstVersionString,
           Noctomorph::Controller::createInstance)

END_FACTORY
