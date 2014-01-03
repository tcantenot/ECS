SRC_DIR = ./src/
OBJ_DIR = ./obj/
DEP_DIR = ./dep/
HDR_DIR = ./include/
BIN_DIR = ./bin

SOURCES = $(shell find $(SRC_DIR) -type f -name \*.cpp)
OBJECTS = $(subst $(SRC_DIR), $(OBJ_DIR), $(SOURCES:%.cpp=%.o))
DEPS    = $(subst $(OBJ_DIR), $(DEP_DIR), $(OBJECTS:%.o=%.d))

BINARY = hybrid_ecs.a
TARGET = $(BIN_DIR)/$(BINARY)

WARNINGS.cpp.debug  = -W -Wall -Wextra -pedantic -Wold-style-cast -Woverloaded-virtual \
				   	  -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-align \
				   	  -Wconversion -Wredundant-decls -Winit-self -Wswitch-default \
				   	  -Wswitch-enum -Wundef -Winline -Wunused-parameter -Weffc++ -Werror

WARNINGS =#$(WARNINGS.cpp.debug)

DEBUG   = debug
RELEASE = release
FAST 	= fast
PROFILE = profile
CLEAN   = clean
STATIC  = static
FAST_DEBUG = fastdebug

GMON_FILE = ./gmon.out


HDR_FLAGS = $(foreach dir, $(HDR_DIR), -I$(dir))
CFLAGS  = -std=c++11 $(WARNINGS)
LDFLAGS = -std=c++11 $(shell sdl2-config --cflags) $(shell sdl2-config --libs) -lSDL2_image 

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	@echo -e "\nCompiling < $< >..."
	@mkdir -p $(@D)
	@mkdir -p $(dir $(DEP_DIR)$*.d)
	g++ $(CFLAGS) $(HDR_FLAGS) -c $< -MMD -MF $(DEP_DIR)$*.d -o $@ 

 
$(FAST): CFLAGS  += -DNDEBUG -DHYBRID_ECS_FAST -O3
$(FAST): LDFLAGS += -O3
$(FAST): $(STATIC)

$(RELEASE): CFLAGS  += -DNDEBUG -O3
$(RELEASE): LDFLAGS += -O3
$(RELEASE): $(STATIC)

$(DEBUG): CFLAGS  += -g3 -O0 -pg
$(DEBUG): LDFLAGS += -O0 -pg
$(DEBUG): WARNINGS = $(WARNINGS.cpp.debug)
$(DEBUG): $(STATIC)

$(FAST_DEBUG): CFLAGS  += -g3 -O0 -pg -DHYBRID_ECS_FAST
$(FAST_DEBUG): LDFLAGS += -O0 -pg
$(FAST_DEBUG): $(STATIC)

$(PROFILE): CFLAGS  += -g3 -O0 -pg
$(PROFILE): LDFLAGS += -O0 -pg
$(PROFILE): $(STATIC)

$(STATIC): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	ar rcs $(TARGET) $(OBJECTS)

$(CLEAN):
	rm -r $(OBJECTS) $(DEPS) $(TARGET) 2> /dev/null
