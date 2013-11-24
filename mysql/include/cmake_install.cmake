# Install script for directory: /root/mysql-connector-c-6.1.2-src/include

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local/mysql")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
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

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/root/mysql-connector-c-6.1.2-src/include/mysql.h"
    "/root/mysql-connector-c-6.1.2-src/include/mysql_com.h"
    "/root/mysql-connector-c-6.1.2-src/include/mysql_time.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_list.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_alloc.h"
    "/root/mysql-connector-c-6.1.2-src/include/typelib.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_dbug.h"
    "/root/mysql-connector-c-6.1.2-src/include/m_string.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_sys.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_xml.h"
    "/root/mysql-connector-c-6.1.2-src/include/mysql_embed.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_pthread.h"
    "/root/mysql-connector-c-6.1.2-src/include/decimal.h"
    "/root/mysql-connector-c-6.1.2-src/include/errmsg.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_global.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_net.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_getopt.h"
    "/root/mysql-connector-c-6.1.2-src/include/sslopt-longopts.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_dir.h"
    "/root/mysql-connector-c-6.1.2-src/include/sslopt-vars.h"
    "/root/mysql-connector-c-6.1.2-src/include/sslopt-case.h"
    "/root/mysql-connector-c-6.1.2-src/include/sql_common.h"
    "/root/mysql-connector-c-6.1.2-src/include/keycache.h"
    "/root/mysql-connector-c-6.1.2-src/include/m_ctype.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_attribute.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_compiler.h"
    "/root/mysql-connector-c-6.1.2-src/include/mysql_com_server.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_byteorder.h"
    "/root/mysql-connector-c-6.1.2-src/include/byte_order_generic.h"
    "/root/mysql-connector-c-6.1.2-src/include/byte_order_generic_x86.h"
    "/root/mysql-connector-c-6.1.2-src/include/byte_order_generic_x86_64.h"
    "/root/mysql-connector-c-6.1.2-src/include/little_endian.h"
    "/root/mysql-connector-c-6.1.2-src/include/big_endian.h"
    "/root/mysql-connector-c-6.1.2-src/include/mysql_version.h"
    "/root/mysql-connector-c-6.1.2-src/include/my_config.h"
    "/root/mysql-connector-c-6.1.2-src/include/mysqld_ername.h"
    "/root/mysql-connector-c-6.1.2-src/include/mysqld_error.h"
    "/root/mysql-connector-c-6.1.2-src/include/sql_state.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mysql" TYPE DIRECTORY FILES "/root/mysql-connector-c-6.1.2-src/include/mysql/" REGEX "/[^/]*\\.h$" REGEX "/psi\\_abi[^/]*$" EXCLUDE)
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Development")

