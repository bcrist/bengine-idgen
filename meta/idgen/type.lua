include 'common/enum'

type = make_enum_class('be::idgen::Type', 'U8', {
   'signed_dec','unsigned_dec', 'hex', 'base64', 'hash'
})

include('common/enum_std_begin', type)
include('common/enum_std_end', type)
