include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/AddFilterMacro.cmake)
include (code/CIncludeLists.cmake)

include (code/audio/CMakeLists.cmake)
include (code/containers/CMakeLists.cmake)
include (code/content/CMakeLists.cmake)
include (code/error/CMakeLists.cmake)
include (code/game/CMakeLists.cmake)
include (code/graphics/CMakeLists.cmake)
include (code/gui/CMakeLists.cmake)
include (code/io/CMakeLists.cmake)
include (code/math/CMakeLists.cmake)
include (code/memory/CMakeLists.cmake)
include (code/networking/CMakeLists.cmake)
include (code/physics/CMakeLists.cmake)
include (code/plugin/CMakeLists.cmake)
include (code/tools/CMakeLists.cmake)
include (code/thread/CMakeLists.cmake)
  
addFilter(CODE_SRCS src code/
            event.h
            event_internal.h
            HappyEngine.h           HappyEngine.cpp
            HappyFunctions.h        HappyFunctions.cpp
            HappyPCH.h              HappyPCH.cpp
            HappyHelpers.h          HappyHelpers.cpp
            HappyTargetDefines.h
            HappyTypes.h            HappyTypes.cpp
            HappyGuid.h             HappyGuid.cpp
            HeString.h              HeString.cpp
            FixedString.h           FixedString.cpp
            FixedStringMap.h
            HeFixedStringTable.h
            HeFixedStrings.h        HeFixedStrings.cpp
            GlobalSettings.h        GlobalSettings.cpp
            GlobalStringTable.h     GlobalStringTable.cpp
            OpenGL.h                OpenGL.cpp
            RTTI.h
            Singleton.h
            StaticDataManager.h     StaticDataManager.cpp
            Timer.cpp               Timer.h
        )
                        
set (HAPPYENGINE_CODE_SRCS  ${CODE_SRCS})
                            