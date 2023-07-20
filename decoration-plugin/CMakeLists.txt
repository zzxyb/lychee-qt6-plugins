set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(ECM REQUIRED NO_MODULE)
find_package(Qt6 6.5 COMPONENTS Svg Gui WaylandClient)

qt_standard_project_setup()

add_library(LycheeWaylandDecorationPlugin)
target_sources(LycheeWaylandDecorationPlugin PRIVATE
    main.cpp

    bradient.json
)

set_target_properties(LycheeWaylandDecorationPlugin PROPERTIES
    VERSION     0.1
    EXPORT_NAME lychee-decoration
)

target_link_libraries(LycheeWaylandDecorationPlugin
    PRIVATE
        Qt6::Svg
        Qt6::Gui
        Qt6::GuiPrivate
        Qt::WaylandClientPrivate
)

if (TARGET Qt6::DBus)
  target_link_libraries(LycheeWaylandDecorationPlugin PRIVATE Qt6::DBus)
endif()

install(TARGETS LycheeWaylandDecorationPlugin DESTINATION /usr/lib/qt6/plugins/wayland-decoration-client)

