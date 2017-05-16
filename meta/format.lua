include 'common/enum'

format = make_enum_class('be::idgen::Format', 'U8', {
   'standard', 'canonical', 'tab', 'json', 'xml'
})

include('common/enum_std_begin', format)
include('common/enum_std_end', format)
