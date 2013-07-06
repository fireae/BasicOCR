# Install script for directory: E:/dev/BasicOCR/libharu

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/libharu")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libharu" TYPE FILE FILES
    "E:/dev/BasicOCR/libharu/include/hpdf.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_encoder.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_image.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_streams.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_annotation.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_encrypt.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_info.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_types.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_catalog.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_encryptdict.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_list.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_u3d.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_conf.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_error.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_mmgr.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_utils.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_ext_gstate.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_objects.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_version.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_consts.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_font.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_outline.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_destination.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_fontdef.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_page_label.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_doc.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_gstate.h"
    "E:/dev/BasicOCR/libharu/include/hpdf_pages.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("E:/dev/BasicOCR/libharu/build/src/cmake_install.cmake")
  INCLUDE("E:/dev/BasicOCR/libharu/build/demo/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "E:/dev/BasicOCR/libharu/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "E:/dev/BasicOCR/libharu/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
