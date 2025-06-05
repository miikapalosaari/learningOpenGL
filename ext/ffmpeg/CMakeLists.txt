set(FFMPEG_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include CACHE INTERNAL "")
set(FFMPEG_LIBRARY_DIR ${CMAKE_CURRENT_LIST_DIR}/lib CACHE INTERNAL "")
set(FFMPEG_BIN_DIR ${CMAKE_CURRENT_LIST_DIR}/bin CACHE INTERNAL "")

set(FFMPEG_LIBS
    ${CMAKE_CURRENT_LIST_DIR}/lib/libavcodec.dll.a
    ${CMAKE_CURRENT_LIST_DIR}/lib/libavformat.dll.a
    ${CMAKE_CURRENT_LIST_DIR}/lib/libavutil.dll.a
    ${CMAKE_CURRENT_LIST_DIR}/lib/libswresample.dll.a
    ${CMAKE_CURRENT_LIST_DIR}/lib/libswscale.dll.a
    CACHE INTERNAL ""
)


