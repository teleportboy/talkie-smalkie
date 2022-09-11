TARGET = server
CC = gcc
DEPENDENCIES = -pthread -ljson-c -ldl

CORE_SRC = $(wildcard ./core/*.c) $(wildcard ./core/**/*.c)
DATABASE_SRC = $(wildcard ./database_api/*.c)
UTILITY_SRC = $(wildcard ./utility_features/*.c)
SSE_SRC = $(wildcard ./sse/*.c)



$(TARGET): $(CORE_SRC) $(DATABASE_SRC) $(UTILITY_SRC) $(SSE_SRC)
	$(CC) -g $(CORE_SRC) $(DATABASE_SRC) $(UTILITY_SRC) $(SSE_SRC) -o $(TARGET) $(DEPENDENCIES)

	