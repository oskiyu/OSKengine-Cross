project(Assets NONE)

file(GLOB_RECURSE ALL_RESOURCES LIST_DIRECTORIES true CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Assets/*")
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Assets/" FILES ${ALL_RESOURCES})
list(FILTER ALL_RESOURCES INCLUDE REGEX "\\.(json|hdr|wav|ico|txt|glb|TTF|bat|png|jpg)$")

add_custom_target(Assets SOURCES ${ALL_RESOURCES})




# %VULKAN_SDK%/Bin/glslc.exe shader.vert -DOSK_VULKAN -o shader.vert.spv
project(Shaders NONE)

file(GLOB_RECURSE ALL_SHADERS LIST_DIRECTORIES true CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*")
file(GLOB_RECURSE ALL_MATERIALS LIST_DIRECTORIES true CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Materials/*")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Shaders/"   PREFIX "Shaders"   FILES ${ALL_SHADERS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/Materials/" PREFIX "Materials" FILES ${ALL_MATERIALS})

list(FILTER ALL_SHADERS INCLUDE REGEX "\\.(vert|frag|comp|glsl|rmiss|mesh|rchit)$")
list(FILTER ALL_MATERIALS INCLUDE REGEX "\\.json$")

add_custom_target(Shaders SOURCES ${ALL_SHADERS} ${ALL_MATERIALS})
    
list(FILTER ALL_SHADERS INCLUDE REGEX "\\.(vert|frag|comp|rmiss|mesh|rchit)$") # Quitamos los glsl
foreach(FILE ${ALL_SHADERS})
    get_filename_component(DIR_SHADER ${FILE} DIRECTORY)
    get_filename_component(NAME_SHADER ${FILE} NAME)
    
    # set(OSK_CMD cd ${DIR_SHADER} && ${VULKAN_SDK}/Bin/glslc.exe ${NAME_SHADER} -DOSK_VULKAN --target-env=vulkan1.3 -o ${NAME_SHADER}.spv) 
    set(OSK_CMD ${VULKAN_SDK}/Bin/glslc.exe ${FILE} -DOSK_VULKAN --target-env=vulkan1.3 -o ${FILE}.spv) 
    
    add_custom_command(
        TARGET Shaders
        COMMAND ${OSK_CMD}
        MAIN_DEPENDENCY ${FILE}.spv
        COMMENT "${OSK_CMD}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)
endforeach(FILE)
