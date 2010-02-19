#Almp3 cmake instructions
SET(ALMP3_SRCS almp3.c dct64_i386.c layer3.c common.c decode_i386.c interface.c layer2.c tabinit.c)
ADD_LIBRARY(almp3 STATIC ${ALMP3_SRCS})
TARGET_LINK_LIBRARIES(almp3 alleg)