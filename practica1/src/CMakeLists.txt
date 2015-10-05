INCLUDE_DIRECTORIES("${PROJECT_INCLUDE_DIR}")

SET(COMMON_SRCS 
	Mundo.cpp 
	Esfera.cpp
	Plano.cpp
	Raqueta.cpp
	Vector2D.cpp)
				
ADD_EXECUTABLE(tenis tenis.cpp ${COMMON_SRCS})

TARGET_LINK_LIBRARIES(tenis glut GL GLU)
