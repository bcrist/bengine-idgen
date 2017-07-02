#pragma once
#ifndef BE_SIZEOF_VERSION_HPP_
#define BE_SIZEOF_VERSION_HPP_

#include <be/core/macros.hpp>

#define BE_SIZEOF_VERSION_MAJOR 0
#define BE_SIZEOF_VERSION_MINOR 1
#define BE_SIZEOF_VERSION_REV 3

/*!! include('common/version', 'BE_SIZEOF', 'sizeof') !! 6 */
/* ################# !! GENERATED CODE -- DO NOT MODIFY !! ################# */
#define BE_SIZEOF_VERSION (BE_SIZEOF_VERSION_MAJOR * 100000 + BE_SIZEOF_VERSION_MINOR * 1000 + BE_SIZEOF_VERSION_REV)
#define BE_SIZEOF_VERSION_STRING "sizeof " BE_STRINGIFY(BE_SIZEOF_VERSION_MAJOR) "." BE_STRINGIFY(BE_SIZEOF_VERSION_MINOR) "." BE_STRINGIFY(BE_SIZEOF_VERSION_REV)

/* ######################### END OF GENERATED CODE ######################### */

#endif
