#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// Contains a configuration that sets pixel data format and Image AOI.

#ifndef INCLUDED_PIXELFORMATANDAOICONFIGURATION_H_00104928
#define INCLUDED_PIXELFORMATANDAOICONFIGURATION_H_00104928

#include <pylon/ConfigurationEventHandler.h>
#include <pylon/ParameterIncludes.h>
#include <pylon/PylonIncludes.h>

namespace Pylon {
    class CInstantCamera;
}
class Configuration : public Pylon::CConfigurationEventHandler {
public:
    void OnOpened(Pylon::CInstantCamera& camera);
};

#endif /* INCLUDED_PIXELFORMATANDAOICONFIGURATION_H_00104928 */

#endif // CONFIGURATION_H
