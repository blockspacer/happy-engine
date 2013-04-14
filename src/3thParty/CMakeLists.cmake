include (${HappyEngine_SOURCE_DIR}/cmakeHelpers/LibFindPackage.cmake)

macro (IncludeThirdParty)

if(APPLE)

include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/assimp/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/awesomium/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/boost/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/cairo/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/devIL/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/freeType/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/glew/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/libsndfile/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/openAl/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/physx/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/raknet/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML/include)

add_definitions( -DGLEW_STATIC -DSFML_STATIC -DGLEW_MX -DLLVM )


link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/assimp/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/awesomium/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/boost/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/cairo/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/devIL/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/freeType/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/glew/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/libsndfile/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/openAl/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/physX/lib/osx32)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/rakNet/lib)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML/lib)

else()

include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Assimp/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/include)
if (MSVC11)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost-msvc11/include)
else()
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost/include)
endif()
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Cairo/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/DevIL/include/${PLATFORM}${BITNESS})
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/FreeType/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/FreeType/include/freetype2)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/fontconfig/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/glew/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/libsndfile/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/OpenAl/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/PhysX/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/RakNet/include)
include_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0/include)

add_definitions( -DGLEW_STATIC -DSFML_STATIC -DGLEW_MX -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS )
if (${BITNESS} EQUAL 64)
	add_definitions(-DARCH_64)
else()
	add_definitions(-DARCH_32)
endif()

link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Assimp/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Awesomium/lib/${PLATFORM}${BITNESS})
if (MSVC11)
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost-msvc11/lib/${PLATFORM}${BITNESS})
else()
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Boost/lib/${PLATFORM}${BITNESS})
endif()
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/Cairo/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/DevIL/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/FreeType/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/fontconfig/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/glew/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/libsndfile/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/OpenAl/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/PhysX/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/RakNet/lib/${PLATFORM}${BITNESS})
link_directories(${HappyEngine_SOURCE_DIR}/3thParty/lib/SFML2.0/lib/${PLATFORM}${BITNESS})

endif()

endmacro()

macro (LinkThirdParty target)

if (APPLE)

target_link_libraries(${target} libassimp.dylib)
target_link_libraries(${target} libplugin_carbon_helper.dylib)

target_link_libraries(${target} libboost_chrono-mt.a)
target_link_libraries(${target} libboost_date_time-mt.a)
target_link_libraries(${target} libboost_regex-mt.a)
target_link_libraries(${target} libboost_system-mt.a)
target_link_libraries(${target} libboost_thread-mt.a)
target_link_libraries(${target} libboost_filesystem-mt.a)

target_link_libraries(${target} libIL.a)
target_link_libraries(${target} libILU.a)

target_link_libraries(${target} libfreetype.a)

target_link_libraries(${target} libGLEWmx.a)
target_link_libraries(${target} libsndfile.a)
target_link_libraries(${target} libopenal.dylib)
target_link_libraries(${target} libcairo.a)

target_link_libraries(${target} debug libLowLevelCHECKED.a)
target_link_libraries(${target} debug libLowLevelClothCHECKED.a)
target_link_libraries(${target} debug libPhysX3CharacterKinematicCHECKED.a)
target_link_libraries(${target} debug libPhysX3CHECKED.a)
target_link_libraries(${target} debug libPhysX3CommonCHECKED.a)
target_link_libraries(${target} debug libPhysX3CookingCHECKED.a)
target_link_libraries(${target} debug libPhysX3ExtensionsCHECKED.a)
target_link_libraries(${target} debug libPhysX3VehicleCHECKED.a)
target_link_libraries(${target} debug libPhysXProfileSDKCHECKED.a)
target_link_libraries(${target} debug libPhysXVisualDebuggerSDKCHECKED.a)
target_link_libraries(${target} debug libPxTaskCHECKED.a)
target_link_libraries(${target} debug libRepX3CHECKED.a)
target_link_libraries(${target} debug libRepXUpgrader3CHECKED.a)
target_link_libraries(${target} debug libRepXUpgrader3CHECKED.a)
target_link_libraries(${target} debug libSceneQueryCHECKED.a)
target_link_libraries(${target} debug libSimulationControllerCHECKED.a)

target_link_libraries(${target} optimized libPhysX3CharacterKinematic.a)
target_link_libraries(${target} optimized libPhysX3.a)
target_link_libraries(${target} optimized libPhysX3Common.a)
target_link_libraries(${target} optimized libPhysX3Cooking.a)
target_link_libraries(${target} optimized libPhysX3Extensions.a)
target_link_libraries(${target} optimized libPhysX3Vehicle.a)
target_link_libraries(${target} optimized libPhysXProfileSDK.a)
target_link_libraries(${target} optimized libPhysXVisualDebuggerSDK.a)
target_link_libraries(${target} optimized libPxTask.a)
target_link_libraries(${target} optimized libRepX3.a)
target_link_libraries(${target} optimized libRepXUpgrader3.a)

target_link_libraries(${target} libRakNetLibStatic.a)

target_link_libraries(${target} libsfml-graphics.dylib)
target_link_libraries(${target} libsfml-system.dylib)
target_link_libraries(${target} libsfml-window.dylib)

else()

target_link_libraries(${target} assimp)
target_link_libraries(${target} awesomium)

if (MSVC11)
target_link_libraries(${target} debug libboost_chrono-vc110-mt-gd-1_53)
target_link_libraries(${target} debug libboost_date_time-vc110-mt-gd-1_53)
target_link_libraries(${target} debug libboost_regex-vc110-mt-gd-1_53)
target_link_libraries(${target} debug libboost_system-vc110-mt-gd-1_53)
target_link_libraries(${target} debug libboost_thread-vc110-mt-gd-1_53)
target_link_libraries(${target} debug libboost_filesystem-vc110-mt-gd-1_53)

target_link_libraries(${target} optimized libboost_chrono-vc110-mt-1_53)
target_link_libraries(${target} optimized libboost_date_time-vc110-mt-1_53)
target_link_libraries(${target} optimized libboost_regex-vc110-mt-1_53)
target_link_libraries(${target} optimized libboost_system-vc110-mt-1_53)
target_link_libraries(${target} optimized libboost_thread-vc110-mt-1_53)
target_link_libraries(${target} optimized libboost_filesystem-vc110-mt-1_53)
else()
target_link_libraries(${target} debug libboost_chrono-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_date_time-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_regex-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_system-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_thread-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_filesystem-vc100-mt-gd-1_49)
target_link_libraries(${target} debug libboost_signals-vc100-mt-gd-1_49)

target_link_libraries(${target} optimized libboost_chrono-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_date_time-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_regex-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_system-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_thread-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_filesystem-vc100-mt-1_49)
target_link_libraries(${target} optimized libboost_signals-vc100-mt-1_49)
endif()

target_link_libraries(${target} DevIL)
target_link_libraries(${target} ILU)

target_link_libraries(${target} freetype)

target_link_libraries(${target} debug glew32mxsd)
target_link_libraries(${target} optimized glew32mxs)
target_link_libraries(${target} libsndfile-1)
target_link_libraries(${target} OpenAL32)
target_link_libraries(${target} cairo)

if (${BITNESS} EQUAL 32)
    set (arch x86)
else()
    set (arch x64)
endif()

target_link_libraries(${target} debug PhysX3CharacterKinematicDEBUG_${arch})
target_link_libraries(${target} debug PhysX3DEBUG_${arch})
target_link_libraries(${target} debug PhysX3CommonDEBUG_${arch})
target_link_libraries(${target} debug PhysX3CookingDEBUG_${arch})
target_link_libraries(${target} debug PhysX3ExtensionsDEBUG)
target_link_libraries(${target} debug PhysX3VehicleDEBUG)
target_link_libraries(${target} debug PhysXProfileSDKDEBUG)
target_link_libraries(${target} debug PhysXVisualDebuggerSDKDEBUG)
target_link_libraries(${target} debug PxTaskDEBUG)
target_link_libraries(${target} debug RepX3DEBUG)
target_link_libraries(${target} debug RepXUpgrader3DEBUG)

target_link_libraries(${target} optimized PhysX3CharacterKinematic_${arch})
target_link_libraries(${target} optimized PhysX3_${arch})
target_link_libraries(${target} optimized PhysX3Common_${arch})
target_link_libraries(${target} optimized PhysX3Cooking_${arch})
target_link_libraries(${target} optimized PhysX3Extensions)
target_link_libraries(${target} optimized PhysX3Vehicle)
target_link_libraries(${target} optimized PhysXProfileSDK)
target_link_libraries(${target} optimized PhysXVisualDebuggerSDK)
target_link_libraries(${target} optimized PxTask)
target_link_libraries(${target} optimized RepX3)
target_link_libraries(${target} optimized RepXUpgrader3)

target_link_libraries(${target} debug RakNetStatic-d)
target_link_libraries(${target} optimized RakNetStatic-r)
target_link_libraries(${target} ws2_32)

target_link_libraries(${target} debug sfml-graphics-s-d)
target_link_libraries(${target} debug sfml-main-d)
target_link_libraries(${target} debug sfml-system-s-d)
target_link_libraries(${target} debug sfml-window-s-d)
target_link_libraries(${target} optimized sfml-graphics-s)
target_link_libraries(${target} optimized sfml-main)
target_link_libraries(${target} optimized sfml-system-s)
target_link_libraries(${target} optimized sfml-window-s)

endif()

endmacro()

macro (CopyThirdParty target)

if (APPLE)

else()

file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Debug/${PLATFORM}${BITNESS}/ DESTINATION ${target}/Debug FILES_MATCHING REGEX .*)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Release/${PLATFORM}${BITNESS}/ DESTINATION ${target}/MinSizeRel FILES_MATCHING REGEX .*)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Release/${PLATFORM}${BITNESS}/ DESTINATION ${target}/Release FILES_MATCHING REGEX .*)
file(INSTALL ${HappyEngine_SOURCE_DIR}/3thParty/dll/Release/${PLATFORM}${BITNESS}/ DESTINATION ${target}/RelWithDebInfo FILES_MATCHING REGEX .*)

endif()

endmacro()
