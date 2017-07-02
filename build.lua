tool 'tools-misc' {
   app 'idgen' {
      icon 'icon/bengine-warm.ico',
      src 'src-idgen/*.cpp',
      link_project {
         'core',
         'core-id-with-names',
         'util-fs'
      }
   },
   app 'sizeof' {
      icon 'icon/bengine-warm.ico',
      src 'src-sizeof/*.cpp',
      link_project {
         'core',
         'core-id',
         'util',
         'gfx'
      }
   },
   app 'ccolor' {
      when 'toolchain ~= "vc_win" or configuration ~= "release"' {
         disabled
      },
      icon 'icon/bengine-warm.ico',
      src 'src-ccolor/*.cpp',
      link_project {
         'core',
         'core-id'
      }
   },
   app 'wedo' {
      when 'toolchain ~= "vc_win" or configuration ~= "release"' {
         disabled
      },
      icon 'icon/bengine-warm.ico',
      src 'src-wedo/*.cpp',
      require_admin
   }
}
