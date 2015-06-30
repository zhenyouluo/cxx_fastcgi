#include <Pluma/Connector.hpp>
#include "PrepareStg.h"

/////////////////////////////////////////////////////////
/// Plugin connects to hosts through this function
/// Add Eagle and Jaguar providers to the host, so that it
/// can create and use those kind of warriors
/////////////////////////////////////////////////////////

using namespace smart_plugin_frame;

PLUMA_CONNECTOR
bool connectPlugin(pluma::Host& host){
    host.add( new PrepareStgProvider() );
    return true;
}
