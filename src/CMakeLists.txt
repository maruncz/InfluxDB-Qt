add_library(InfluxDB-Qt STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/InfluxDB-Qt/influxdb.h influxdb.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/InfluxDB-Qt/point.h point.cpp
  httpinterface.h httpinterface.cpp
)

target_link_libraries(InfluxDB-Qt
    PUBLIC
    Qt${QT_VERSION_MAJOR}::Network
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core)

target_include_directories(InfluxDB-Qt
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/InfluxDB-Qt)

target_compile_definitions(InfluxDB-Qt PRIVATE INFLUXDBQT_LIBRARY)
