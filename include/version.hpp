#pragma once
#ifndef BE_IDGEN_VERSION_HPP_
#define BE_IDGEN_VERSION_HPP_

#include <be/core/macros.hpp>

#define BE_IDGEN_VERSION_MAJOR 0
#define BE_IDGEN_VERSION_MINOR 1
#define BE_IDGEN_VERSION_REV 2

/*!! include('common/version', 'BE_IDGEN', 'idgen') !! 6 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#define BE_IDGEN_VERSION (BE_IDGEN_VERSION_MAJOR * 100000 + BE_IDGEN_VERSION_MINOR * 1000 + BE_IDGEN_VERSION_REV)
#define BE_IDGEN_VERSION_STRING "idgen " BE_STRINGIFY(BE_IDGEN_VERSION_MAJOR) "." BE_STRINGIFY(BE_IDGEN_VERSION_MINOR) "." BE_STRINGIFY(BE_IDGEN_VERSION_REV)

/* ######################### END OF GENERATED CODE ######################### */

#endif
