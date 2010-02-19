#src dir cmake file
SET(GND_SRC intro.c jogo.c)
ADD_EXECUTABLE(gnd ${GND_SRC})

add_subdirectory(libs)
TARGET_LINK_LIBRARIES(gnd alleg)
TARGET_LINK_LIBRARIES(gnd almp3)
