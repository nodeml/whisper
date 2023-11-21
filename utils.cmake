
function(DownloadWhisperCpp VERSION DESTINATION)
  if(NOT EXISTS ${DESTINATION}/whisper)
    execute_process(
      COMMAND git clone --depth 1 --branch v${VERSION} https://github.com/ggerganov/whisper.cpp ${DESTINATION}/whisper
    )
  endif()
endfunction()

function(GenerateNodeLIB)
  if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
    # Generate node.lib
    execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
  endif()
endfunction(GenerateNodeLIB)
