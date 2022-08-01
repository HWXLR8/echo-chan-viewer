$(shell mkdir -p bin)
NAME = echo-chan-viewer

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	LDFLAGS = `pkg-config --static --libs glfw3` -lGL -lm -lXrandr -lXi -lX11 -lXxf86vm -lpthread -ldl -lXinerama -lXcursor
endif
ifeq ($(UNAME_S),Darwin)
	LDFLAGS = `pkg-config --static --libs glfw3` -framework OpenGL -lm -lpthread -ldl
endif

CXXFLAGS = -std=c++17 -Wall -Wextra
CFLAGS = -O2 -Iinclude
CPPFLAGS = -Iinclude
SRC_DIR := src
OBJ_DIR := bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

$(NAME): $(OBJ_FILES) bin/glad.o bin/stb_image.o
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

bin/glad.o: src/glad.c
	gcc $(CFLAGS) -c src/glad.c -o bin/glad.o

bin/stb_image.o: src/stb_image.c
	gcc $(CFLAGS) -c src/stb_image.c -o bin/stb_image.o

.PHONY: run
run: $(NAME)
	./$(NAME)

.PHONY: clean
clean:
	rm -rf bin/*
	rm -f $(NAME)
