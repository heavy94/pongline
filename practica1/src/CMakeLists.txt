INCLUDE_DIRECTORIES("${PROJECT_INCLUDE_DIR}")
find_package (Threads)
add_compile_options(-fpermissive)
SET(COMMON_SRCS 
	Esfera.cpp
	Plano.cpp
	Raqueta.cpp
	Vector2D.cpp
	Socket.cpp)
				
ADD_EXECUTABLE(cliente cliente.cpp MundoCliente.cpp ${COMMON_SRCS})
TARGET_LINK_LIBRARIES(cliente glut GL GLU)
ADD_EXECUTABLE(servidor servidor.cpp MundoServidor.cpp ${COMMON_SRCS})
TARGET_LINK_LIBRARIES(servidor glut GL GLU ${CMAKE_THREAD_LIBS_INIT})
