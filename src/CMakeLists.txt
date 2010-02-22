#src dir cmake file
SET(GND_SRC alleg.c aux.c game.c gnd.c visual.c mainboard.c recordes.c)
ADD_EXECUTABLE(gnd ${GND_SRC})

#Allegro lib
TARGET_LINK_LIBRARIES(gnd alleg)

#Logg libs
TARGET_LINK_LIBRARIES(gnd logg)
TARGET_LINK_LIBRARIES(gnd vorbisfile)
TARGET_LINK_LIBRARIES(gnd vorbis)
TARGET_LINK_LIBRARIES(gnd ogg)