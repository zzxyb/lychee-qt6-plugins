set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(ECM REQUIRED NO_MODULE)
find_package(Qt6 6.4 COMPONENTS Svg Gui)

add_library(LycheePlatformThemePlugin
    main.cpp
    lycheeplatformtheme.h
    lycheeplatformtheme.cpp
    qxdgdesktopportalfiledialog_p.h
    qxdgdesktopportalfiledialog.cpp
)

target_sources(LycheePlatformThemePlugin PRIVATE
    lycheePlatformTheme.json
)

set_target_properties(LycheePlatformThemePlugin PROPERTIES
    VERSION     0.1
    EXPORT_NAME lychee-platformt-heme
)

target_link_libraries(LycheePlatformThemePlugin
    PRIVATE
        Qt6::Svg
        Qt6::Gui
        Qt6::GuiPrivate
)

if (TARGET Qt6::DBus)
  target_link_libraries(LycheePlatformThemePlugin PRIVATE Qt6::DBus)
endif()

install(TARGETS LycheePlatformThemePlugin DESTINATION /usr/lib/qt6/plugins/wayland-decoration-client)

