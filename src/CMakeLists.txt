set (SOURCES
    main.cpp
    operators.cpp
    reader.cpp
    frame.cpp
    evaluator.cpp
    interpreter.cpp
    printer.cpp
)

add_library(${PROJECT_NAME}_lib STATIC ${SOURCES})
