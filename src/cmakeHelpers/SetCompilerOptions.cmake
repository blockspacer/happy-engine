
if(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv|nmake)")
    add_definitions(-W4 -MP)                     #Warning 4, multiprocess compilation
    add_definitions(-fp:precise -arch:SSE2 )     #floating points, SSE2 support
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
    
    # Release specific
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ot -GS- -D_HAS_ITERATOR_DEBUGGING=0 -D_SECURE_SCL=0 -GR-")
    
    # Min Size Release specific
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -D_HAS_ITERATOR_DEBUGGING=0 -D_SECURE_SCL=0 -GR-")
    
    # Rel With Deb Info specific
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -GR")
    
    # Debug specific
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -WX -GR")
    
endif()