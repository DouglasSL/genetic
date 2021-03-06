#
# Build libraries
#

# Input handler
ADD_LIBRARY(tetris_input input.h input.cpp)

# Output handler
ADD_LIBRARY(tetris_output output.h output.cpp)
FIND_PACKAGE(SDL REQUIRED)
TARGET_LINK_LIBRARIES(tetris_output ${SDL_LIBRARY})
TARGET_LINK_LIBRARIES(tetris_output "SDL_gfx")

# Pieces
ADD_LIBRARY(tetris_pieces pieces.h pieces.cpp)

# Board
ADD_LIBRARY(tetris_board board.h board.cpp)
TARGET_LINK_LIBRARIES(tetris_board tetris_pieces)

# Game
ADD_LIBRARY(tetris_game game.h game.cpp)
TARGET_LINK_LIBRARIES(tetris_game tetris_output)
TARGET_LINK_LIBRARIES(tetris_game tetris_pieces)
TARGET_LINK_LIBRARIES(tetris_game tetris_board)


#
# Interactive client
#

# Executable
ADD_EXECUTABLE(tetris_interactive tetris_interactive.cpp)
TARGET_LINK_LIBRARIES(tetris_interactive tetris_input)
TARGET_LINK_LIBRARIES(tetris_interactive tetris_output)
TARGET_LINK_LIBRARIES(tetris_interactive tetris_pieces)
TARGET_LINK_LIBRARIES(tetris_interactive tetris_board)
TARGET_LINK_LIBRARIES(tetris_interactive tetris_game)



#
# Evolving client
#

# Executable
ADD_EXECUTABLE(tetris tetris.cpp)
TARGET_LINK_LIBRARIES(tetris tetris_input tetris_output tetris_pieces tetris_board tetris_game)
TARGET_LINK_LIBRARIES(tetris parser)
TARGET_LINK_LIBRARIES(tetris dna population environment)

