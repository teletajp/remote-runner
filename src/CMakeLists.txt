
add_executable(${PROJECT_NAME})

find_package(fmt CONFIG REQUIRED)
#add_subdirectory(import/core/src)


target_link_libraries(${PROJECT_NAME}
PRIVATE
	fmt::fmt
	)

set_target_properties(${PROJECT_NAME}
PROPERTIES
	CXX_STANDARD 14
	CMAKE_CXX_STANDARD_REQUIRED on)



target_include_directories(${PROJECT_NAME}
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	./tools
	)



target_compile_options(${PROJECT_NAME}
PRIVATE 
	$<$<CXX_COMPILER_ID:GNU>:-Werror> # Флаги для GNU компилятора
	$<$<CXX_COMPILER_ID:MSVC>: -D_CRT_SECURE_NO_WARNINGS -DHAVE_REMOTE /Zi /std:c++14>
	#$<$<CXX_COMPILER_ID:MSVC>:/W3 /WX /MT -D_CRT_SECURE_NO_WARNINGS -DHAVE_REMOTE>
	) # Флаги для MSVC компилятора

set_target_properties(${PROJECT_NAME}
PROPERTIES LINK_FLAGS /DEBUG
)

target_sources(${PROJECT_NAME}
PRIVATE
	version.h
	version.h.in
	main.cpp
	./remote_runner/remote_runner.h
	./remote_runner/remote_runner.cpp
	./processor/processor.h
	./processor/processor.cpp
	./interfaces/iserver_engine.h
	./interfaces/iprocessor.h
	./server/select_base_server_engine.h
	./server/newtork_initializer.h
	./server/newtork_initializer.cpp
	./server/tcp_server.h
	./server/tcp_server.cpp
	./server/connection.h
	./server/connection.cpp
	./server/listener.h
	./server/listener.cpp
	)

target_sources(${PROJECT_NAME}
PRIVATE
	./tools/thread/runnable.h
	./tools/thread/runnable.cpp
	./tools/logger/ilogger.h
	./tools/logger/mt_logger.h
	./tools/logger/mt_logger.cpp
	./tools/locker/spinlock_mutex.h
	./tools/locker/spinlock_mutex.cpp
	./tools/cash_timer/cash_timer.h
	./tools/cash_timer/cash_timer.cpp
#	../tools/fifo/fifo.h
#	../tools/fifo/fifo.cpp
#	../tools/fifo/collector.h
#	../tools/fifo/collector.cpp
#	../tools/logger/mt_logger.cpp
#	../tools/logger/mt_logger.h
#	../tools/hexutil/hexutil.cpp
#	../tools/hexutil/hexutil.h
#	../common/rrprotocol.h
#	../common/rrprotocol.cpp
)


