disable_rtti()

set(SOURCES src/ConvertUTF.c)

new_library_static(SimpleIni "${SOURCES}" "")

target_include_directories(SimpleIni_static PUBLIC "include")

set_target_properties(SimpleIni_static PROPERTIES FOLDER Libraries)
