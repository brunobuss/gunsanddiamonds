#src dir cmake file
SET(GND_SRC intro.c jogo.c)
ADD_EXECUTABLE(gnd ${GND_SRC})

add_subdirectory(libs)

#Logg libs
TARGET_LINK_LIBRARIES(gnd logg)
TARGET_LINK_LIBRARIES(gnd vorbisfile)
TARGET_LINK_LIBRARIES(gnd vorbis)
TARGET_LINK_LIBRARIES(gnd ogg)

#Allegro lib
TARGET_LINK_LIBRARIES(gnd alleg)

#Almp3 lib
TARGET_LINK_LIBRARIES(gnd almp3)
