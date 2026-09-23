SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
RAYLIB := external/raylib/src/libraylib.a

$(BUILD_DIR)/gomoku: $(OBJS)
	g++ $(OBJS) $(RAYLIB) -lX11 -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -c $< \
		-MMD -MP -I$(INC_DIR) \
		-Iexternal/raylib/src \
		-o $@

clean:
	rm -rf build/
