cmake_minimum_required(VERSION 3.0.0)
project(server VERSION 0.1.0 LANGUAGES C CXX)


project(client VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()


#1 client - 1 server
# add_executable(server server/runserver.cpp server/server.cpp file/fileIO.cpp encryption/cypher.cpp)

# add_executable(client client/runclient.cpp client/client.cpp file/fileIO.cpp encryption/cypher.cpp)



# target_link_libraries(client jsoncpp cryptopp)

# target_link_libraries(server jsoncpp cryptopp)

# target_include_directories(client PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
#                             $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/file>
#                             $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/encryption>)
                           
# target_include_directories(server PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
#                             $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/file>
#                             $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/encryption>)


#multiple clients - 1 server
add_executable(server  multiclientmodel/server/server.cpp multiclientmodel/server/main.cpp file/fileIO.cpp  encryption/cypher.cpp)

add_executable(client  multiclientmodel/client/client.cpp multiclientmodel/client/main.cpp file/fileIO.cpp encryption/cypher.cpp)


target_link_libraries(client jsoncpp cryptopp)

target_link_libraries(server jsoncpp cryptopp)

target_include_directories(client PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/file>
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/encryption>)
                           
target_include_directories(server PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/>
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/file>
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/encryption>)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
