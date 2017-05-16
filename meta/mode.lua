include 'common/enum'

mode = make_enum_class('be::idgen::Mode', 'U8', {
   'fnv0', 'fnv1', 'fnv1a'
})

include('common/enum_std_begin', mode)
include('common/enum_std_end', mode)
