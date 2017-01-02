tool { name = 'idgen',
   projects = {
      app {
         icon = 'icon/bengine-warm.ico',
         src = {
            'src/*.cpp'
         },
         libs = {
            'core',
            'core-id-with-names'
         }
      }
   }
}
